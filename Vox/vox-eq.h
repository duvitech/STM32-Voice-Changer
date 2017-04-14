#ifndef __VOX_EQ_H
#define __VOX_EQ_H

/*
 һ����һ���ֱ�߶ѳ�������֭������
 */

#include "vox.h"
#include "vox-fft.h"

typedef struct {
	float f1, gain1; // ����Ƶ�ʺ�����
	float f2, gain2; // �ҵ��Ƶ�ʺ�����
	
	// �����Ϊ˽�����ԣ��� vox_eq_compile_line ���ɡ���Ҫֱ���޸ģ�
	float _k;
	int _i1, _i2;
} vox_eq_line_t;

typedef struct {
	vox_eq_line_t *lines;
	int count;
} vox_eq_t;

#define vox_eq_from_lines(lines)  ((vox_eq_t){(lines), sizeof(lines)/sizeof(vox_eq_line_t)})

void vox_eq_compile_line(vox_eq_line_t *line);
void vox_eq_compile(vox_eq_t *eq);

void vox_eq_apply(vox_eq_t *eq, vox_fft_t *fft);

#endif
