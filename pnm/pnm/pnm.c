//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//      �t�@�C����      :       pnm.c
//      �T�v            :       PNM�`��(PBM, PGM, PPM�`��)�摜���������C�u����
//--------------------------------------------------------------------------------
#include "pnm.h"

static PNM *_read_pnm_pbm_ascii(FILE *);

static PNM *_read_pnm_pgm_ascii(FILE *);

static PNM *_read_pnm_ppm_ascii(FILE *);

static PNM *_read_pnm_pbm_bin(FILE *);

static PNM *_read_pnm_pgm_bin(FILE *);

static PNM *_read_pnm_ppm_bin(FILE *);

static int _write_pnm_pbm_ascii(PNM *, FILE *);

static int _write_pnm_pgm_ascii(PNM *, FILE *);

static int _write_pnm_ppm_ascii(PNM *, FILE *);

static int _write_pnm_pbm_bin(PNM *, FILE *);

static int _write_pnm_pgm_bin(PNM *, FILE *);

static int _write_pnm_ppm_bin(PNM *, FILE *);


//----------------------------------------------------------------------------
// �֐���       �Fclose_pnm
// �T�v         �FPNM�f�[�^�̈�J��
// �߂�l       �Fint
// ����         �FPNM *pnm
//                PNM�f�[�^
//----------------------------------------------------------------------------
int close_pnm(PNM *pnm)
{
	free(pnm->r);
	free(pnm->g);
	free(pnm->b);
	free(pnm);

	return 0;
}

//----------------------------------------------------------------------------
// �֐���       �Fread_pnm
// �T�v         �FPNM�摜�̓ǂݍ���
// �߂�l       �FPNM *
// ����         �FFILE *fp
//                �摜�t�@�C���̃t�@�C���|�C���^
//----------------------------------------------------------------------------
PNM *read_pnm(FILE *fp)
{
	PNM *pnm;
	unsigned char buff[1024];
	int type;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	rewind(fp);

	if (buff[0] != 'P') return NULL;

	sscanf(buff, "P%d", &type);

	switch (type) {

	case PBMASCII:
		pnm = _read_pnm_pbm_ascii(fp);
		break;

	case PGMASCII:
		pnm = _read_pnm_pgm_ascii(fp);
		break;

	case PPMASCII:
		pnm = _read_pnm_ppm_ascii(fp);
		break;

	case PBMBIN:
		pnm = _read_pnm_pbm_bin(fp);
		break;

	case PGMBIN:
		pnm = _read_pnm_pgm_bin(fp);
		break;

	case PPMBIN:
		pnm = _read_pnm_ppm_bin(fp);
		break;

	default:
		pnm = NULL;
	}

	return pnm;

}

//----------------------------------------------------------------------------
// �֐���       �Fwrite_pnm
// �T�v         �FPNM�摜�̕ۑ�
// �߂�l       �Fint
// ����         �FFILE *fp
//                �ۑ��t�@�C���̃t�@�C���|�C���^
// ����         �FPNM *pnm
//                PNM�f�[�^
//----------------------------------------------------------------------------
int write_pnm(PNM *pnm, FILE *fp)
{
	switch (pnm->type) {
	case PBMASCII:
		_write_pnm_pbm_ascii(pnm, fp);
		break;

	case PGMASCII:
		_write_pnm_pgm_ascii(pnm, fp);
		break;

	case PPMASCII:
		_write_pnm_ppm_ascii(pnm, fp);
		break;

	case PBMBIN:
		_write_pnm_pbm_bin(pnm, fp);
		break;

	case PGMBIN:
		_write_pnm_pgm_bin(pnm, fp);
		break;

	case PPMBIN:
		_write_pnm_ppm_bin(pnm, fp);
		break;
	}

	return 0;
}


static PNM *_read_pnm_pbm_ascii(FILE *fp)
{
	PNM *pnm;
	unsigned char buff[1024];
	int type;
	int tmpwidth;
	int tmpheight;
	int i;
	int j;
	int tmp;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	if (buff[0] != 'P') return  NULL;                                 /* �t�@�C���`�����Ⴄ */

	sscanf(buff, "P%d", &type);

	do {
		fgets(buff, 255, fp);
	} while (buff[0] == '#');

	sscanf(buff, "%d%d", &tmpwidth, &tmpheight);
	if (tmpwidth < 1 || tmpheight < 1) return NULL;                                  /* �T�C�Y���ُ� */

	pnm = (PNM *)malloc(sizeof(PNM));
	if (pnm == NULL) return NULL;

	/* �f�[�^�� */

	pnm->type = PBMASCII;
	pnm->width = tmpwidth;
	pnm->height = tmpheight;
	pnm->r = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->g = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->b = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	if (pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) {
		close_pnm(pnm);
		return NULL;
	}
	for (i = 0; i<pnm->height; i++) {
		for (j = 0; j<pnm->width; j++) {
			if (fscanf(fp, "%1d", &tmp) != 1) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->r + i * pnm->width + j) = *(pnm->g + i * pnm->width + j) = *(pnm->b + i * pnm->width + j) = (unsigned char)((tmp) * 255);
		}
	}

	return pnm;
}

static PNM *_read_pnm_pgm_ascii(FILE *fp)
{
	PNM *pnm;
	unsigned char buff[1024];
	int type;
	int tmpwidth;
	int tmpheight;
	int i;
	int j;
	int tmp;
	int max;
	int r;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	if (buff[0] != 'P') return  NULL;                                 /* �t�@�C���`�����Ⴄ */

	sscanf(buff, "P%d", &type);

	do {
		fgets(buff, 255, fp);
	} while (buff[0] == '#');

	sscanf(buff, "%d%d", &tmpwidth, &tmpheight);
	if (tmpwidth < 1 || tmpheight < 1) return NULL;                                  /* �T�C�Y���ُ� */

	do {
		fgets(buff, 1024, fp);
	} while (buff[0] == '#');       /* �R�����g�ǂݔ�΂� */

	sscanf(buff, "%d", &max);
	if (max < 1 || max > 255) return NULL;                                /* �ő�P�x���͈͊O */
	r = 255 / (float)max;

	pnm = (PNM *)malloc(sizeof(PNM));
	if (pnm == NULL) return NULL;

	pnm->type = PGMASCII;
	pnm->width = tmpwidth;
	pnm->height = tmpheight;
	pnm->r = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->g = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->b = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	if (pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) {
		close_pnm(pnm);
		return NULL;
	}
	for (i = 0; i<pnm->height; i++) {
		for (j = 0; j<pnm->width; j++) {
			if (fscanf(fp, "%d", &tmp) != 1) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->r + i * pnm->width + j) = *(pnm->g + i * pnm->width + j) = *(pnm->b + i * pnm->width + j) = (unsigned char)(tmp * r);
		}
	}

	return pnm;

}

static PNM *_read_pnm_ppm_ascii(FILE *fp)
{
	PNM *pnm;
	unsigned char buff[1024];
	int type;
	int tmpwidth;
	int tmpheight;
	int i;
	int j;
	int tmp;
	int max;
	int r;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	if (buff[0] != 'P') return  NULL;                                 /* �t�@�C���`�����Ⴄ */

	sscanf(buff, "P%d", &type);

	do {
		fgets(buff, 255, fp);
	} while (buff[0] == '#');

	sscanf(buff, "%d%d", &tmpwidth, &tmpheight);
	if (tmpwidth < 1 || tmpheight < 1) return NULL;                                  /* �T�C�Y���ُ� */

	do {
		fgets(buff, 1024, fp);
	} while (buff[0] == '#');       /* �R�����g�ǂݔ�΂� */

	sscanf(buff, "%d", &max);
	if (max < 1 || max > 255) return NULL;                                /* �ő�P�x���͈͊O */
	r = 255 / (float)max;

	/* �f�[�^�� */

	pnm = (PNM *)malloc(sizeof(PNM));
	if (pnm == NULL) return NULL;

	pnm->type = PPMASCII;
	pnm->width = tmpwidth;
	pnm->height = tmpheight;
	pnm->r = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width * pnm->height);
	pnm->g = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width * pnm->height);
	pnm->b = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width * pnm->height);

	if (pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) {
		close_pnm(pnm);
		return NULL;
	}

	for (i = 0; i<pnm->height; i++) {
		for (j = 0; j<pnm->width; j++) {
			if (fscanf(fp, "%d", &tmp) != 1) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->r + i * pnm->width + j) = (unsigned char)(tmp * r);

			if (fscanf(fp, "%d", &tmp) != 1) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->g + i * pnm->width + j) = (unsigned char)(tmp * r);

			if (fscanf(fp, "%d", &tmp) != 1) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->b + i * pnm->width + j) = (unsigned char)(tmp * r);
		}
	}

	return pnm;

}

static PNM *_read_pnm_pbm_bin(FILE *fp)
{
	/* ������ */
	PNM *pnm;
	unsigned char buff[1024];
	int type;
	int tmpwidth;
	int tmpheight;
	int i;
	int j;
	int tmp;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	if (buff[0] != 'P') return  NULL;                                 /* �t�@�C���`�����Ⴄ */

	sscanf(buff, "P%d", &type);

	do {
		fgets(buff, 255, fp);
	} while (buff[0] == '#');

	sscanf(buff, "%d%d", &tmpwidth, &tmpheight);
	if (tmpwidth < 1 || tmpheight < 1) return NULL;                                  /* �T�C�Y���ُ� */

																					 /* �f�[�^�� */

	pnm = (PNM *)malloc(sizeof(PNM));
	if (pnm == NULL) return NULL;

	pnm->type = PBMBIN;
	pnm->width = tmpwidth;
	pnm->height = tmpheight;
	pnm->r = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->g = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->b = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);

	if (pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) {
		close_pnm(pnm);
		return NULL;
	}

	for (i = 0; i<pnm->height; i++) {
		for (j = 0; j<pnm->width; j++) {
			if (fscanf(fp, "%d", &tmp) != 1) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->r + i * pnm->width + j) = *(pnm->g + i * pnm->width + j) = *(pnm->b + i * pnm->width + j) = (unsigned char)((tmp) * 255);
		}
	}

	return pnm;

}

static PNM *_read_pnm_pgm_bin(FILE *fp)
{
	/* ������ */
	PNM *pnm;
	unsigned char buff[1024];
	int type;
	int tmpwidth;
	int tmpheight;
	int i;
	int j;
	int tmp;
	int max;
	int r;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	if (buff[0] != 'P') return  NULL;                                 /* �t�@�C���`�����Ⴄ */

	sscanf(buff, "P%d", &type);

	do {
		fgets(buff, 255, fp);
	} while (buff[0] == '#');

	sscanf(buff, "%d%d", &tmpwidth, &tmpheight);
	if (tmpwidth < 1 || tmpheight < 1) return NULL;                                  /* �T�C�Y���ُ� */

	do {
		fgets(buff, 1024, fp);
	} while (buff[0] == '#');       /* �R�����g�ǂݔ�΂� */

	sscanf(buff, "%d", &max);
	if (max < 1 || max > 255) return NULL;                                /* �ő�P�x���͈͊O */
	r = 255 / (float)max;

	pnm = (PNM *)malloc(sizeof(PNM));
	if (pnm == NULL) return NULL;

	pnm->type = PGMBIN;
	pnm->width = tmpwidth;
	pnm->height = tmpheight;
	pnm->r = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->g = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->b = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	if (pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) {
		close_pnm(pnm);
		return NULL;
	}

	for (i = 0; i<pnm->height; i++) {
		for (j = 0; j<pnm->width; j++) {
			if ((tmp = getc(fp)) == EOF) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->r + i * pnm->width + j) = *(pnm->g + i * pnm->width + j) = *(pnm->b + i * pnm->width + j) = (unsigned char)(tmp * r);
		}
	}

	return pnm;

}

static PNM *_read_pnm_ppm_bin(FILE *fp)
{
	PNM *pnm;
	unsigned char buff[1024];
	int type;
	int tmpwidth;
	int tmpheight;
	int i;
	int j;
	int tmp;
	int max;
	int r;

	if (fp == NULL) return NULL;

	fgets(buff, 255, fp);
	if (buff[0] != 'P') return  NULL;                                 /* �t�@�C���`�����Ⴄ */

	sscanf(buff, "P%d", &type);

	do {
		fgets(buff, 255, fp);
	} while (buff[0] == '#');

	sscanf(buff, "%d%d", &tmpwidth, &tmpheight);
	if (tmpwidth < 1 || tmpheight < 1) return NULL;                                  /* �T�C�Y���ُ� */

	do {
		fgets(buff, 1024, fp);
	} while (buff[0] == '#');       /* �R�����g�ǂݔ�΂� */

	sscanf(buff, "%d", &max);
	if (max < 1 || max > 255) return NULL;                                /* �ő�P�x���͈͊O */
	r = 255 / (float)max;

	/* �f�[�^�� */

	pnm = (PNM *)malloc(sizeof(PNM));
	if (pnm == NULL) return NULL;

	pnm->type = PPMBIN;
	pnm->width = tmpwidth;
	pnm->height = tmpheight;
	pnm->r = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->g = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);
	pnm->b = (unsigned char*)malloc(sizeof(unsigned char) * pnm->width *pnm->height);

	if (pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) {
		close_pnm(pnm);
		return NULL;
	}

	for (i = 0; i<pnm->height; i++) {
		for (j = 0; j<pnm->width; j++) {
			if ((tmp = getc(fp)) == EOF) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->r + i * pnm->width + j) = (unsigned char)(tmp * r);
			if ((tmp = getc(fp)) == EOF) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->g + i * pnm->width + j) = (unsigned char)(tmp * r);
			if ((tmp = getc(fp)) == EOF) {
				close_pnm(pnm);
				return NULL;
			}
			*(pnm->b + i * pnm->width + j) = (unsigned char)(tmp * r);
		}
	}

	return pnm;

}

static int _write_pnm_pbm_ascii(PNM *pnm, FILE *fp)
{
	char buff[1024];
	int buffsize;
	int i;
	int j;

	/* FIXME: format�C���q(�T�C�Y) */
	buffsize = snprintf(buff, 10, "P%d\n", PBMASCII);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d %d\n", pnm->width, pnm->height);
	fwrite(buff, buffsize, 1, fp);


	for (i = 0; i<pnm->height; i++)
	{
		for (j = 0; j<pnm->width; j++)
		{
			buffsize = snprintf(buff, 5, "%d\n", (pnm->r[i*pnm->width + j] + pnm->b[i*pnm->width + j] + pnm->g[i*pnm->width + j]) / 3 / 255);
			fwrite(buff, buffsize, 1, fp);
		}

	}

	return 0;
}


static int _write_pnm_pgm_ascii(PNM *pnm, FILE *fp)
{
	char buff[1024];
	int buffsize;
	int i;
	int j;

	/* FIXME: format�C���q(�T�C�Y) */
	buffsize = snprintf(buff, 10, "P%d\n", PGMASCII);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d %d\n", pnm->width, pnm->height);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d\n", 255);
	fwrite(buff, buffsize, 1, fp);


	for (i = 0; i<pnm->height; i++)
	{
		for (j = 0; j<pnm->width; j++)
		{
			buffsize = snprintf(buff, 5, "%d\n", (pnm->r[i*pnm->width + j] + pnm->b[i*pnm->width + j] + pnm->g[i*pnm->width + j]) / 3);
			fwrite(buff, buffsize, 1, fp);
		}

	}

	return 0;
}

static int _write_pnm_ppm_ascii(PNM *pnm, FILE *fp)
{
	char buff[1024];
	int buffsize;
	int i;
	int j;

	/* FIXME: format�C���q(�T�C�Y) */
	buffsize = snprintf(buff, 10, "P%d\n", PPMASCII);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d %d\n", pnm->width, pnm->height);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d\n", 255);
	fwrite(buff, buffsize, 1, fp);


	for (i = 0; i<pnm->height; i++)
	{
		for (j = 0; j<pnm->width; j++)
		{
			buffsize = snprintf(buff, 5, "%d\n", pnm->r[i*pnm->width + j]);
			fwrite(buff, buffsize, 1, fp);
			buffsize = snprintf(buff, 5, "%d\n", pnm->g[i*pnm->width + j]);
			fwrite(buff, buffsize, 1, fp);
			buffsize = snprintf(buff, 5, "%d\n", pnm->b[i*pnm->width + j]);
			fwrite(buff, buffsize, 1, fp);
		}

	}

	return 0;
}


static int _write_pnm_pbm_bin(PNM *pnm, FILE *fp)
{
	char buff[1024];
	int buffsize;
	int i;


	/* FIXME: format�C���q(�T�C�Y) */
	buffsize = snprintf(buff, 10, "P%d\n", PBMBIN);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d %d\n", pnm->width, pnm->height);
	fwrite(buff, buffsize, 1, fp);
	/*
	for(i=0; i<pnm->height * pnm->width; i++)
	{
	buffsize = snprintf(buff, 5, "%d\n", (pnm->r[i*pnm->width + j] + pnm->b[i*pnm->width + j] + pnm->g[i*pnm->width + j]) / 3 / 255);
	fwrite(buff, buffsize, 1, fp);
	}
	*/
	return 0;
}


static int _write_pnm_pgm_bin(PNM *pnm, FILE *fp)
{
	char buff[1024];
	int buffsize;
	int i;
	unsigned char tmpb;


	/* FIXME: format�C���q(�T�C�Y) */
	buffsize = snprintf(buff, 10, "P%d\n", PGMBIN);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d %d\n", pnm->width, pnm->height);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d\n", 255);
	fwrite(buff, buffsize, 1, fp);

	for (i = 0; i < pnm->height * pnm->width; i++)
	{
		tmpb = (pnm->r[i] + pnm->b[i] + pnm->g[i]) / 3;
		fwrite(&tmpb, 1, 1, fp);
	}

	return 0;
}


static int _write_pnm_ppm_bin(PNM *pnm, FILE *fp)
{
	char buff[1024];
	int buffsize;
	int i;
	unsigned char tmpb;


	/* FIXME: format�C���q(�T�C�Y) */
	buffsize = snprintf(buff, 10, "P%d\n", PPMBIN);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d %d\n", pnm->width, pnm->height);
	fwrite(buff, buffsize, 1, fp);

	buffsize = snprintf(buff, 20, "%d\n", 255);
	fwrite(buff, buffsize, 1, fp);

	for (i = 0; i < pnm->height * pnm->width; i++)
	{
		fwrite(&(pnm->r[i]), 1, 1, fp);
		fwrite(&(pnm->g[i]), 1, 1, fp);
		fwrite(&(pnm->b[i]), 1, 1, fp);
	}

	return 0;
}

