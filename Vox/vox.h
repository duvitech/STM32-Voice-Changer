/*
 ��֮ Voice Process
 
 ���ݸ�ʽΪ Q15������Ϊ 512
 */

#ifndef _VoX_H_
#define _VoX_H_

#include <stdint.h>

#define VOX_SPLFREQ 48000  // ����Ƶ��
#define VOX_BUFLEN  1024   // ���ݴ���������

typedef struct {
	enum vox_buf_status_t {
		VOX_FILLING, VOX_PROCESSING, VOX_PROCESSED,
	} status;
	uint32_t len; // length (count) that filled with data.
	uint16_t data[VOX_BUFLEN];
}	vox_buf_t;

#define VOX_BUFCNT 4
extern vox_buf_t vox_bufs[VOX_BUFCNT];

void vox_init(void);
void vox_cycle(void);
void vox_feed(uint16_t *data, uint32_t len);

// not-implemented in vox.c   return 0 if successfully play.
int vox_play(vox_buf_t *buf);

#endif
