/*
 ��֮ Voice Process
 
 ���ݸ�ʽΪ Q15������Ϊ ��VOX_SPLLEN*2��
	 ǰ һ�� �������ݣ��� һ�� ���������ݡ�����Ϊ�˱����ν�ʱ���ë�̡�
	 ���ŵ�ʱ��� 1/4 ��ʼ�����Ҳ���ǰ��� 1/4:(1/4 + VOX_CROSSOVER) �� fade-in
 */

#ifndef _VoX_H_
#define _VoX_H_

#include <stdint.h>

#define VOX_SPLFREQ 32000   // ����Ƶ�� 48000,32000,16000
#define VOX_SPLLEN  1024    // ÿһ��ʵ��{��ȡ��|Ҫ���ŵ�}�����ĳ���
#define VOX_BUFLEN  (VOX_SPLLEN*2)   // ���ݴ���������
#define VOX_CROSSOVER 48    // ����Ч���Ĳ�����������Ҫ���� (VOX_SPLLEN/2)

typedef struct {
	enum vox_buf_status_t {
		VOX_FILLING, VOX_PROCESSING, VOX_PROCESSED,
	} status;
	uint32_t len; // length filled; length to be played
	uint32_t playOffset; // where playing begins
	int16_t data[VOX_BUFLEN];
}	vox_buf_t;

#define VOX_BUFCNT 5
extern vox_buf_t vox_bufs[VOX_BUFCNT];

void vox_init(void);
void vox_cycle(void);
void vox_feed(uint16_t *data, uint32_t len);

// not-implemented in vox.c   return 0 if successfully play.
int vox_play(vox_buf_t *buf);

#endif
