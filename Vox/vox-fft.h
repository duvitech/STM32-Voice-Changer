/*
 * �ܶ���Ч������Ҫ�õ� FFT�������������ͷ�ļ��ṩ�ĺ�������������Ĺ��̡�
 *
 * ��Ҫע��������������´��һ���ϴδ�������ݣ�Ϊ�˱������Ȳ����ϲ����µ������
 */

#ifndef _VOX_FFT_H
#define _VOX_FFT_H

#include "vox.h"

#define VOX_FFTLEN (VOX_BUFLEN)

#define VOX_FFTRES ((float)VOX_SPLFREQ / VOX_FFTLEN)  // FFT �ֱ��ʣ�ÿһ��Ԫ�ر�ʾ��Ƶ�ʿ�ȣ���λHz

typedef struct _vox_complex {
	float real, imag;
} vox_complex_t;

// ��ʼ����ֻ��Ҫһ��
void vox_init_fft(void);

typedef struct {
	vox_complex_t fft[VOX_FFTLEN];   // ��Ҫ����� FFT ��������
	float timeShift; // ������ʱ����ǰ����
	float timeRatio; // ������������볤�ȵĶ��ٱ��� ��Ҫʹ�ã�����
} vox_fft_t;

// ÿһ���ֻأ�ֻ��Ҫ���Ρ���
/* 1. ����������             */ void vox_fft_begin(vox_fft_t *fft, const vox_buf_t *buf);
/* 2. ����                   */
/*   - ���������ƣ��������  */   void vox_fft_phase_shift(vox_fft_t *fft, float delta); // ���鲻Ҫ��
/* 3. ifft�ý��             */ void vox_fft_end(vox_fft_t *fft, vox_buf_t *buf);

// һЩ�����õõ��ļ��㹤��
#define VOX_ABS(x) ((x)>0?(x):-(x))
#define VOX_FLOOR(x)  ((int)(x))
#define VOX_CEIL(x)  ((int)(0.999999f+(x)))

void vox_fft_interpolate(const vox_fft_t *fft, float f, vox_complex_t *out); // ���Բ�ֵ��ĳ��Ƶ�ʣ�����Ǹ���

#endif
