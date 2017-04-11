/**
 * SensorTile Voice Process
 *
 *  (C) 2017 laobubu
 *
 */

#include <string.h>

#include "vox.h"
#include "vox-memcpy.h"

int vox_init_pitch(void);
int vox_proc_pitch(vox_buf_t *buf);

vox_buf_t vox_bufs[VOX_BUFCNT];
static vox_buf_t *buf_filling = NULL;

static voxmc_handle_t voxmc[2];

static int vox_process(vox_buf_t *buf) {
	vox_proc_pitch(buf);
	return 0;
}

static int __vox_play(vox_buf_t *buf) {
	// ��� crossover
	static int16_t lastProcessed[VOX_CROSSOVER];
	int16_t *src1 = lastProcessed,
					*src2 = buf->data + buf->playOffset; //src2 == dst
	for (int i=1;i<=VOX_CROSSOVER;i++) {
		register int32_t s1 = *src1, s2 = *src2, out;
		out = (( s2 * i ) + ( s1 * (VOX_CROSSOVER - i) )) / VOX_CROSSOVER;
		*src2 = out;
		
		src1++, src2++;
	}
	
	memcpy(lastProcessed, buf->data + buf->playOffset + buf->len, sizeof(lastProcessed));
	
	// Ȼ�󲥷�
	return vox_play(buf);
}

/*
 * ��ʼ��
 */
void vox_init() {
	memset(vox_bufs, 0, sizeof(vox_bufs));
	buf_filling = vox_bufs;
	
	voxmc_init(&voxmc[0]);
	voxmc_init(&voxmc[1]);
	
	vox_init_pitch();
}

/* 
 * ���µ��������ݽ���ʱ�õ�
 * 
 * data   	��������
 * len    	���ݸ�������λ�ǡ������������ֽ�
 */
void vox_feed(uint16_t *data, uint32_t len) {
	static int16_t old_spl_data[VOX_SPLLEN] = {0};
	
	if (buf_filling->status != VOX_FILLING) return;
	
	uint32_t 	len1 = VOX_SPLLEN - buf_filling->len, // ʵ��Ҫ������short����
						len2 = 0; // �������δ���������
	if (len > len1) { len2 = len - len1; } // �������������е��
	else { len1 = len; }
	
	memcpy(buf_filling->data + VOX_SPLLEN + buf_filling->len, data, len1*2);
	buf_filling->len += len1;
	
	/* ��� buffer �Ƿ��Ѿ������㹻�������ݣ� */
	if (buf_filling->len >= VOX_SPLLEN) {
		// ���ϴβ��������� old_spl_data ������ buf ��ͷ
		memcpy(buf_filling->data, old_spl_data, VOX_SPLLEN*2);
		
		// Ȼ��������²ɵ����ݱ��浽 old_spl_data
		// ʹ�� vox-memcpy �� DMA �첽����
		voxmc_memcpy(&voxmc[0], old_spl_data, buf_filling->data + VOX_SPLLEN, VOX_SPLLEN*2); 
		
		// ���ó�ʼ������Ȼ����Ϊ���Կ�ʼ������
		buf_filling->playOffset = VOX_SPLLEN/2;
		buf_filling->status = VOX_PROCESSING;
		
		// �ƶ�ָ�뵽��һ�� buf
		if (++buf_filling >= &vox_bufs[VOX_BUFCNT]) buf_filling = vox_bufs;
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
			if (0 == __vox_play(buf)) {
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
