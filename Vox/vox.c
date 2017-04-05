/**
 * SensorTile Voice Process
 *
 *  (C) 2017 laobubu
 *
 * ���� Q15 ��ʽ���㣬����/�������Ϊ 1024 ��Ԫ�ء�
 */

#include <string.h>

#include "vox.h"

int vox_init_pitch(void);
int vox_proc_pitch(vox_buf_t *buf);

vox_buf_t vox_bufs[VOX_BUFCNT];
static vox_buf_t *buf_filling = NULL;

static int vox_process(vox_buf_t *buf) {
	vox_proc_pitch(buf);
	return 0;
}

/*
 * ��ʼ��
 */
void vox_init() {
	memset(vox_bufs, 0, sizeof(vox_bufs));
	buf_filling = vox_bufs;
	
	vox_init_pitch();
}

/* 
 * ���µ��������ݽ���ʱ�õ�
 * 
 * data   	��������
 * len    	���ݸ�������λ�ǡ������������ֽ�
 */
void vox_feed(uint16_t *data, uint32_t len) {
	if (buf_filling->status != VOX_FILLING) return;
	
	uint32_t 	len1 = VOX_BUFLEN - buf_filling->len, // ʵ��Ҫ������short����
						len2 = 0; // �������δ���������
	if (len > len1) { len2 = len - len1; } // �������������е��
	else { len1 = len; }
	
	memcpy(buf_filling->data + buf_filling->len, data, len1 * 2);
	buf_filling->len += len1;
	
	/* ��� buffer �Ƿ��Ѿ������� */
	if (buf_filling->len >= VOX_BUFLEN) {
		buf_filling->status = VOX_PROCESSING; // ���Ϊ���Կ�ʼ������
		if (++buf_filling >= &vox_bufs[VOX_BUFCNT]) buf_filling = vox_bufs; // �ƶ�ָ�뵽��һ�� buf
	}
	
	if (len2) vox_feed(data + len1, len2);
}

/*
 * ���� main �в���ѭ�����á����ȿ�ʼ�������һ���Ѿ����Բ��ŵ�buf��Ȼ���ٴ�����Դ���һ��buf
 */
void vox_cycle() {
	vox_buf_t *buf;
	
  buf = vox_bufs;
	while (buf != &vox_bufs[VOX_BUFCNT]) {
		if (buf->status == VOX_PROCESSED) { 
			if (0 == vox_play(buf)) {
				buf->len = 0;
				buf->status = VOX_FILLING;
			} 
			break;
		}
		buf++;
	}
	
  buf = vox_bufs;
	while (buf != &vox_bufs[VOX_BUFCNT]) {
		if (buf->status == VOX_PROCESSING) { 
			if (0 == vox_process(buf)) {
				buf->status = VOX_PROCESSED;
			} 
			break;
		}
		buf++;
	}
}
