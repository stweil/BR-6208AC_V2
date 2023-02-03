#define MD5_SIZE_VS_SPEED 2
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# define ALIGN1 __attribute__((aligned(1)))
# define FAST_FUNC __attribute__((regparm(3),stdcall))
#define FF(b, c, d) (d ^ (b & (c ^ d)))
#define FG(b, c, d) FF(d, b, c)
#define FH(b, c, d) (b ^ c ^ d)
#define FI(b, c, d) (c ^ (b | ~d))
#define rotl32(w, s) (((w) << (s)) | ((w) >> (32 - (s))))

//#ifdef L_ENDIAN // Little-Endian system (ex. PC) use this
#define SWAP32(x) (x)
//#else // Big-Endian system (ex. Realtek) use this
//#define SWAP32(x) (((x&0x000000ff)<<24) | ((x&0x0000ff00)<<8) | ((x&0x00ff0000)>>8) | ((x&0xff000000)>>24))
//#endif

const char bb_hexdigits_upcase[] ALIGN1 = "0123456789ABCDEF";

void* FAST_FUNC xmalloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr == NULL && size != 0)
		printf("memory exhausted\n");
	return ptr;
}

ssize_t FAST_FUNC safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while (n < 0 /*&& errno == EINTR*/);

	return n;
}

char* FAST_FUNC bin2hex(char *p, const char *cp, int count)
{
	while (count) {
		unsigned char c = *cp++;
		/* put lowercase hex digits */
		*p++ = 0x20 | bb_hexdigits_upcase[c >> 4];
		*p++ = 0x20 | bb_hexdigits_upcase[c & 0xf];
		count--;
	}
	return p;
}

typedef struct md5_ctx_t {
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
	uint64_t total;
	uint32_t buflen;
	char buffer[128];
} md5_ctx_t;

void FAST_FUNC md5_hash(const void *buffer, size_t len, md5_ctx_t *ctx);
void FAST_FUNC md5_begin(md5_ctx_t *ctx);
void FAST_FUNC md5_end(void *resbuf, md5_ctx_t *ctx);

void FAST_FUNC md5_begin(md5_ctx_t *ctx)
{
	ctx->A = 0x67452301;
	ctx->B = 0xefcdab89;
	ctx->C = 0x98badcfe;
	ctx->D = 0x10325476;
	ctx->total = 0;
	ctx->buflen = 0;
}

static void md5_hash_block(const void *buffer, md5_ctx_t *ctx)
{
	uint32_t correct_words[16];
	const uint32_t *words = buffer;

#if MD5_SIZE_VS_SPEED > 0
	static const uint32_t C_array[] = {
		/* round 1 */
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		/* round 2 */
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x2441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		/* round 3 */
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		/* round 4 */
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};
	static const char P_array[] ALIGN1 = {
# if MD5_SIZE_VS_SPEED > 1
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,	/* 1 */
# endif
		1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,	/* 2 */
		5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,	/* 3 */
		0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9	/* 4 */
	};
# if MD5_SIZE_VS_SPEED > 1
	static const char S_array[] ALIGN1 = {
		7, 12, 17, 22,
		5, 9, 14, 20,
		4, 11, 16, 23,
		6, 10, 15, 21
	};
# endif	/* MD5_SIZE_VS_SPEED > 1 */
#endif
	uint32_t A = ctx->A;
	uint32_t B = ctx->B;
	uint32_t C = ctx->C;
	uint32_t D = ctx->D;

	/* Process all bytes in the buffer with 64 bytes in each round of
	   the loop.  */
	uint32_t *cwp = correct_words;
	uint32_t A_save = A;
	uint32_t B_save = B;
	uint32_t C_save = C;
	uint32_t D_save = D;

#if MD5_SIZE_VS_SPEED > 1
	const uint32_t *pc;
	const char *pp;
	const char *ps;
	int i;
	uint32_t temp;

	for (i = 0; i < 16; i++)
		cwp[i] = SWAP32(words[i]);
	words += 16;

# if MD5_SIZE_VS_SPEED > 2
	pc = C_array;
	pp = P_array;
	ps = S_array - 4;

	for (i = 0; i < 64; i++) {
		if ((i & 0x0f) == 0)
			ps += 4;
		temp = A;
		switch (i >> 4) {
		case 0:
			temp += FF(B, C, D);
			break;
		case 1:
			temp += FG(B, C, D);
			break;
		case 2:
			temp += FH(B, C, D);
			break;
		case 3:
			temp += FI(B, C, D);
		}
		temp += cwp[(int) (*pp++)] + *pc++;
		temp = rotl32(temp, ps[i & 3]);
		temp += B;
		A = D;
		D = C;
		C = B;
		B = temp;
	}
# else
	pc = C_array;
	pp = P_array;
	ps = S_array;

	for (i = 0; i < 16; i++) {
		temp = A + FF(B, C, D) + cwp[(int) (*pp++)] + *pc++;
		temp = rotl32(temp, ps[i & 3]);
		temp += B;
		A = D;
		D = C;
		C = B;
		B = temp;
	}
	ps += 4;
	for (i = 0; i < 16; i++) {
		temp = A + FG(B, C, D) + cwp[(int) (*pp++)] + *pc++;
		temp = rotl32(temp, ps[i & 3]);
		temp += B;
		A = D;
		D = C;
		C = B;
		B = temp;
	}
	ps += 4;
	for (i = 0; i < 16; i++) {
		temp = A + FH(B, C, D) + cwp[(int) (*pp++)] + *pc++;
		temp = rotl32(temp, ps[i & 3]);
		temp += B;
		A = D;
		D = C;
		C = B;
		B = temp;
	}
	ps += 4;
	for (i = 0; i < 16; i++) {
		temp = A + FI(B, C, D) + cwp[(int) (*pp++)] + *pc++;
		temp = rotl32(temp, ps[i & 3]);
		temp += B;
		A = D;
		D = C;
		C = B;
		B = temp;
	}

# endif /* MD5_SIZE_VS_SPEED > 2 */
#else
	/* First round: using the given function, the context and a constant
	   the next context is computed.  Because the algorithms processing
	   unit is a 32-bit word and it is determined to work on words in
	   little endian byte order we perhaps have to change the byte order
	   before the computation.  To reduce the work for the next steps
	   we store the swapped words in the array CORRECT_WORDS.  */
# define OP(a, b, c, d, s, T) \
	do { \
		a += FF(b, c, d) + (*cwp++ = SWAP32(*words)) + T; \
		++words; \
		a = rotl32(a, s); \
		a += b; \
	} while (0)

	/* Before we start, one word to the strange constants.
	   They are defined in RFC 1321 as
	   T[i] = (int)(4294967296.0 * fabs(sin(i))), i=1..64
	 */

# if MD5_SIZE_VS_SPEED == 1
	const uint32_t *pc;
	const char *pp;
	int i;
# endif	/* MD5_SIZE_VS_SPEED */

	/* Round 1.  */
# if MD5_SIZE_VS_SPEED == 1
	pc = C_array;
	for (i = 0; i < 4; i++) {
		OP(A, B, C, D, 7, *pc++);
		OP(D, A, B, C, 12, *pc++);
		OP(C, D, A, B, 17, *pc++);
		OP(B, C, D, A, 22, *pc++);
	}
# else
	OP(A, B, C, D, 7, 0xd76aa478);
	OP(D, A, B, C, 12, 0xe8c7b756);
	OP(C, D, A, B, 17, 0x242070db);
	OP(B, C, D, A, 22, 0xc1bdceee);
	OP(A, B, C, D, 7, 0xf57c0faf);
	OP(D, A, B, C, 12, 0x4787c62a);
	OP(C, D, A, B, 17, 0xa8304613);
	OP(B, C, D, A, 22, 0xfd469501);
	OP(A, B, C, D, 7, 0x698098d8);
	OP(D, A, B, C, 12, 0x8b44f7af);
	OP(C, D, A, B, 17, 0xffff5bb1);
	OP(B, C, D, A, 22, 0x895cd7be);
	OP(A, B, C, D, 7, 0x6b901122);
	OP(D, A, B, C, 12, 0xfd987193);
	OP(C, D, A, B, 17, 0xa679438e);
	OP(B, C, D, A, 22, 0x49b40821);
# endif/* MD5_SIZE_VS_SPEED == 1 */

	/* For the second to fourth round we have the possibly swapped words
	   in CORRECT_WORDS.  Redefine the macro to take an additional first
	   argument specifying the function to use.  */
# undef OP
# define OP(f, a, b, c, d, k, s, T) \
	do { \
		a += f(b, c, d) + correct_words[k] + T; \
		a = rotl32(a, s); \
		a += b; \
	} while (0)

	/* Round 2.  */
# if MD5_SIZE_VS_SPEED == 1
	pp = P_array;
	for (i = 0; i < 4; i++) {
		OP(FG, A, B, C, D, (int) (*pp++), 5, *pc++);
		OP(FG, D, A, B, C, (int) (*pp++), 9, *pc++);
		OP(FG, C, D, A, B, (int) (*pp++), 14, *pc++);
		OP(FG, B, C, D, A, (int) (*pp++), 20, *pc++);
	}
# else
	OP(FG, A, B, C, D, 1, 5, 0xf61e2562);
	OP(FG, D, A, B, C, 6, 9, 0xc040b340);
	OP(FG, C, D, A, B, 11, 14, 0x265e5a51);
	OP(FG, B, C, D, A, 0, 20, 0xe9b6c7aa);
	OP(FG, A, B, C, D, 5, 5, 0xd62f105d);
	OP(FG, D, A, B, C, 10, 9, 0x02441453);
	OP(FG, C, D, A, B, 15, 14, 0xd8a1e681);
	OP(FG, B, C, D, A, 4, 20, 0xe7d3fbc8);
	OP(FG, A, B, C, D, 9, 5, 0x21e1cde6);
	OP(FG, D, A, B, C, 14, 9, 0xc33707d6);
	OP(FG, C, D, A, B, 3, 14, 0xf4d50d87);
	OP(FG, B, C, D, A, 8, 20, 0x455a14ed);
	OP(FG, A, B, C, D, 13, 5, 0xa9e3e905);
	OP(FG, D, A, B, C, 2, 9, 0xfcefa3f8);
	OP(FG, C, D, A, B, 7, 14, 0x676f02d9);
	OP(FG, B, C, D, A, 12, 20, 0x8d2a4c8a);
# endif/* MD5_SIZE_VS_SPEED == 1 */

	/* Round 3.  */
# if MD5_SIZE_VS_SPEED == 1
	for (i = 0; i < 4; i++) {
		OP(FH, A, B, C, D, (int) (*pp++), 4, *pc++);
		OP(FH, D, A, B, C, (int) (*pp++), 11, *pc++);
		OP(FH, C, D, A, B, (int) (*pp++), 16, *pc++);
		OP(FH, B, C, D, A, (int) (*pp++), 23, *pc++);
	}
# else
	OP(FH, A, B, C, D, 5, 4, 0xfffa3942);
	OP(FH, D, A, B, C, 8, 11, 0x8771f681);
	OP(FH, C, D, A, B, 11, 16, 0x6d9d6122);
	OP(FH, B, C, D, A, 14, 23, 0xfde5380c);
	OP(FH, A, B, C, D, 1, 4, 0xa4beea44);
	OP(FH, D, A, B, C, 4, 11, 0x4bdecfa9);
	OP(FH, C, D, A, B, 7, 16, 0xf6bb4b60);
	OP(FH, B, C, D, A, 10, 23, 0xbebfbc70);
	OP(FH, A, B, C, D, 13, 4, 0x289b7ec6);
	OP(FH, D, A, B, C, 0, 11, 0xeaa127fa);
	OP(FH, C, D, A, B, 3, 16, 0xd4ef3085);
	OP(FH, B, C, D, A, 6, 23, 0x04881d05);
	OP(FH, A, B, C, D, 9, 4, 0xd9d4d039);
	OP(FH, D, A, B, C, 12, 11, 0xe6db99e5);
	OP(FH, C, D, A, B, 15, 16, 0x1fa27cf8);
	OP(FH, B, C, D, A, 2, 23, 0xc4ac5665);
# endif/* MD5_SIZE_VS_SPEED == 1 */

	/* Round 4.  */
# if MD5_SIZE_VS_SPEED == 1
	for (i = 0; i < 4; i++) {
		OP(FI, A, B, C, D, (int) (*pp++), 6, *pc++);
		OP(FI, D, A, B, C, (int) (*pp++), 10, *pc++);
		OP(FI, C, D, A, B, (int) (*pp++), 15, *pc++);
		OP(FI, B, C, D, A, (int) (*pp++), 21, *pc++);
	}
# else
	OP(FI, A, B, C, D, 0, 6, 0xf4292244);
	OP(FI, D, A, B, C, 7, 10, 0x432aff97);
	OP(FI, C, D, A, B, 14, 15, 0xab9423a7);
	OP(FI, B, C, D, A, 5, 21, 0xfc93a039);
	OP(FI, A, B, C, D, 12, 6, 0x655b59c3);
	OP(FI, D, A, B, C, 3, 10, 0x8f0ccc92);
	OP(FI, C, D, A, B, 10, 15, 0xffeff47d);
	OP(FI, B, C, D, A, 1, 21, 0x85845dd1);
	OP(FI, A, B, C, D, 8, 6, 0x6fa87e4f);
	OP(FI, D, A, B, C, 15, 10, 0xfe2ce6e0);
	OP(FI, C, D, A, B, 6, 15, 0xa3014314);
	OP(FI, B, C, D, A, 13, 21, 0x4e0811a1);
	OP(FI, A, B, C, D, 4, 6, 0xf7537e82);
	OP(FI, D, A, B, C, 11, 10, 0xbd3af235);
	OP(FI, C, D, A, B, 2, 15, 0x2ad7d2bb);
	OP(FI, B, C, D, A, 9, 21, 0xeb86d391);
# endif	/* MD5_SIZE_VS_SPEED == 1 */
#endif	/* MD5_SIZE_VS_SPEED > 1 */

	/* Add the starting values of the context.  */
	A += A_save;
	B += B_save;
	C += C_save;
	D += D_save;

	/* Put checksum in context given as argument.  */
	ctx->A = A;
	ctx->B = B;
	ctx->C = C;
	ctx->D = D;
}

void FAST_FUNC md5_hash(const void *buffer, size_t len, md5_ctx_t *ctx)
{
	char *buf = (char *)buffer;

	/* RFC 1321 specifies the possible length of the file up to 2^64 bits,
	 * Here we only track the number of bytes.  */
	ctx->total += len;

	/* Process all input. */
	while (len) {
		unsigned i = 64 - ctx->buflen;

		/* Copy data into aligned buffer. */
		if (i > len) i = len;
		memcpy(ctx->buffer + ctx->buflen, buf, i);
		len -= i;
		ctx->buflen += i;
		buf += i;

		/* When buffer fills up, process it. */
		if (ctx->buflen == 64) {
			md5_hash_block(ctx->buffer, ctx);
			ctx->buflen = 0;
		}
	}
}

void FAST_FUNC md5_end(void *resbuf, md5_ctx_t *ctx)
{
	char *buf = ctx->buffer;
	int i;

	/* Pad data to block size.  */
	buf[ctx->buflen++] = 0x80;
	memset(buf + ctx->buflen, 0, 128 - ctx->buflen);

	/* Put the 64-bit file length in *bits* at the end of the buffer.  */
	ctx->total <<= 3;
	if (ctx->buflen > 56)
		buf += 64;
	for (i = 0; i < 8; i++)
		buf[56 + i] = ctx->total >> (i*8);

	/* Process last bytes.  */
	if (buf != ctx->buffer)
		md5_hash_block(ctx->buffer, ctx);
	md5_hash_block(buf, ctx);

	/* The MD5 result is in little endian byte order.
	 * We (ab)use the fact that A-D are consecutive in memory.
	 */
	ctx->A = SWAP32(ctx->A);
	ctx->B = SWAP32(ctx->B);
	ctx->C = SWAP32(ctx->C);
	ctx->D = SWAP32(ctx->D);
	memcpy(resbuf, &ctx->A, sizeof(ctx->A) * 4);
}

/* This might be useful elsewhere */
static unsigned char *hash_bin_to_hex(unsigned char *hash_value,
				unsigned hash_length)
{
	/* xzalloc zero-terminates */
	char *hex_value = (char *)xmalloc((hash_length * 2) + 1);
	memset(hex_value, 0, (hash_length * 2) + 1);

	bin2hex(hex_value, (char*)hash_value, hash_length);
	return (unsigned char *)hex_value;
}

void md5_main(char *input, int count, char *output)
{
	int src_fd, hash_len, read_done;
	md5_ctx_t md5;

	uint8_t *hash_value = NULL;
	unsigned char *in_buf = (unsigned char *)xmalloc(4096);
	void FAST_FUNC (*update)(const void*, size_t, void*);
	void FAST_FUNC (*final)(void*, void*);

	md5_begin(&md5);
	update = (void*)md5_hash;
	final = (void*)md5_end;
	hash_len = 16;

	read_done = 0;
	while( count > 0 )
	{
		if(count > 4096)
		{
			memcpy(in_buf, input+read_done, 4096);
			update(in_buf, count, &md5);
			count -= 4096;
			read_done += 4096;
		}
		else
		{
			memcpy(in_buf, input+read_done, count);
			update(in_buf, count, &md5);
			count -= count;
			read_done += count;
		}
	}
	final(in_buf, &md5);
	sprintf(output, hash_bin_to_hex(in_buf, hash_len));
	free(in_buf);
}

//#define _GENERATE_ENCRYPTSEED_USEMD5_
#ifdef _Customer_
	#ifdef _CUSTOMIZE_MODEL_NAME_
		char table[]={ '0','1','2','3','4','5','6','7','8','9'};
		#define NUMBERINTABLE 10//201204016 spring case sw spec modified.
	#else
		char table[]={ '1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H'};
		#define NUMBERINTABLE 17
	#endif //_CUSTOMIZE_MODEL_NAME_

#elif defined(_Customer_)
        char table[]={ 'A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'};
        #define NUMBERINTABLE 24
#elif defined(_Customer_)
        char table[]={  '1','2','3','4','5','6','7','8','9',
                                        'a','b','c','d','e','f','g','h',        'j','k',        'm','n',        'p','q','r','s','t','u','v','w','x','y','z',
                                        'A','B','C','D','E','F','G','H',        'J','K',        'M','N',        'P','Q','R','S','T','U','V','W','X','Y','Z'};
        #define NUMBERINTABLE 55
#else
	char table[]={  	 '2','3','4','5','6','7',    '9',
		      		'a','b','c','d','e','f','g','h',    'j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		      		'A',    'C','D','E','F','G','H','I','J','K','L','M','N',    'P','Q','R','S','T','U','V','W','X','Y','Z'};
	#define NUMBERINTABLE 56
#endif

char mac_addr[]={"001122334455\0"};

#ifdef _GENERATE_ENCRYPTSEED_USEMD5_
char final[16];
#else
char final[9];
#endif

char final_wepmac[14];
char final_wepmac1[200];
int ComputeChecksum(unsigned int input)
{
	unsigned int temp_uint;
	input *= 10;
	temp_uint = 0;
	temp_uint += 3 * ((input / 10000000) % 10);
	temp_uint += 1 * ((input / 1000000) % 10);
	temp_uint += 3 * ((input / 100000) % 10);
	temp_uint += 1 * ((input / 10000) % 10);
	temp_uint += 3 * ((input / 1000) % 10);
	temp_uint += 1 * ((input / 100) % 10);
	temp_uint += 3 * ((input / 10) % 10);
	return (10 - (temp_uint % 10)) % 10;
}

main(int argc, char **argv)
{
	long i;
	char cmd[100];
#ifdef _GENERATE_ENCRYPTSEED_USEMD5_
	unsigned char tmpBuf[33];
   unsigned char digest[16];
#endif
	if(argc < 2) exit(0);

	//if(strcmp("001122334455", argv[1])==0) exit(0);
//EDX ryan		
#if defined(_Customer_) && defined(_BR6675NDSv2_)
	if( !strcmp(argv[1],"Set_Logitec_Ssid") ) {
		char Logitec_ssid[200];
		sprintf(mac_addr, "%s\0", argv[2]);
		sprintf(Logitec_ssid, "flash set SSID logitec-%s", mac_addr+6);
		system(Logitec_ssid);
		sprintf(Logitec_ssid, "flash set INIC_SSID logitec11a-%s", mac_addr+6);
		system(Logitec_ssid);
		#ifdef _MSSID_
		sprintf(Logitec_ssid, "flash set SSID_1 logitec2nd-%s", mac_addr+6);
		system(Logitec_ssid);			
		#endif
		exit(0);
	}
#endif
	
	sprintf(mac_addr, "%s\0", argv[1]);

#ifdef _GENERATE_ENCRYPTSEED_USEMD5_

#if defined(_Customer_)
	FILE *fp;
	unsigned long long int md5_result;
	md5_main(mac_addr, strlen(mac_addr), cmd);
//	printf("%s", cmd);
//	sprintf(cmd, "echo -n \"%s\" | ../busybox-1.15.2/busybox md5sum", mac_addr);
//	sprintf(cmd, "echo -n \"%s\" | md5sum", mac_addr);
//	if( (fp = popen(cmd, "r"))!=NULL )
//	{
//		fgets(cmd, sizeof(cmd), fp);
//		pclose(fp);

		strncpy(tmpBuf, &cmd[24], 8);
		tmpBuf[8]=0;
		md5_result = strtoul(tmpBuf, NULL, 16);

		strncpy(tmpBuf, &cmd[16], 8);
		tmpBuf[8]=0;
		md5_result += (unsigned long long int)strtoul(tmpBuf, NULL, 16) << 32;

//	}
//	else
//		md5_result=0;

	for(i=0; i<12; i++)
	{
		final[i] = table[md5_result % NUMBERINTABLE];
		md5_result /= NUMBERINTABLE;
	}
	final[i]='\0';
#else
   RT_MD5(mac_addr,strlen((char *)mac_addr),digest);
	for(i=0; i<16; i++)
	{
		sprintf((char*)tmpBuf, "%02x", digest[i]);
		sprintf((char*)tmpBuf, "%s%02x", tmpBuf, digest[i+1]);
		final[i] = table[strtol(tmpBuf, NULL, 16) % NUMBERINTABLE];
	}
	final[15]='\0';
#endif

#else
	i = atol(argv[1]+6);
	final[2]=table[((           i+mac_addr[ 3]+mac_addr[ 5])*(mac_addr[ 7]+mac_addr[ 9]+mac_addr[11]))%NUMBERINTABLE];
	final[1]=table[((mac_addr[11]+           i+mac_addr[ 6])*(mac_addr[ 8]+mac_addr[10]+mac_addr[11]))%NUMBERINTABLE];
	final[0]=table[((mac_addr[11]+mac_addr[ 5]+           i)*(mac_addr[ 9]+mac_addr[ 3]+mac_addr[11]))%NUMBERINTABLE];
	final[6]=table[((mac_addr[11]+mac_addr[ 6]+mac_addr[ 8])*(           i+mac_addr[ 4]+mac_addr[11]))%NUMBERINTABLE];
	final[5]=table[((mac_addr[11]+mac_addr[ 3]+mac_addr[ 4])*(mac_addr[ 5]+           i+mac_addr[11]))%NUMBERINTABLE];
	final[3]=table[((mac_addr[11]+mac_addr[ 4]+mac_addr[ 5])*(mac_addr[ 6]+mac_addr[ 7]+           i))%NUMBERINTABLE];
	final[4]=table[((           i+mac_addr[ 6]+mac_addr[ 7])*(mac_addr[ 8]+mac_addr[ 9]+mac_addr[11]))%NUMBERINTABLE];
	final[7]=table[((mac_addr[11]+mac_addr[ 7]+mac_addr[ 8])*(           i+mac_addr[10]+mac_addr[11]))%NUMBERINTABLE];
	final[8]='\0';
#endif

	if( argc==3 && !strcmp(argv[2],"wepmac") )
	{

//		printf("%s\n",final_wepmac);

		char flash_field_name[20];
		strcpy(final_wepmac, final);
		strcat(final_wepmac, final+3);
		#if defined(_BR6425N_) && defined(_Customer_)
			char final_6425_pci[9];
			char final_wepmac_6425_pci[14];
			char table_6425_pci[]={  	 '2','3','4','5','6','7',    '9',
		      		'a','b','c','d','e','f','g','h',    'j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
			#define NUMBERINTABLE_6425_PCI 32

			i = atol(argv[1]+6);
			final_6425_pci[2]=table_6425_pci[((           i+mac_addr[ 3]+mac_addr[ 5])*(mac_addr[ 7]+mac_addr[ 9]+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[1]=table_6425_pci[((mac_addr[11]+           i+mac_addr[ 6])*(mac_addr[ 8]+mac_addr[10]+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[0]=table_6425_pci[((mac_addr[11]+mac_addr[ 5]+           i)*(mac_addr[ 9]+mac_addr[ 3]+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[6]=table_6425_pci[((mac_addr[11]+mac_addr[ 6]+mac_addr[ 8])*(           i+mac_addr[ 4]+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[5]=table_6425_pci[((mac_addr[11]+mac_addr[ 3]+mac_addr[ 4])*(mac_addr[ 5]+           i+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[3]=table_6425_pci[((mac_addr[11]+mac_addr[ 4]+mac_addr[ 5])*(mac_addr[ 6]+mac_addr[ 7]+           i))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[4]=table_6425_pci[((           i+mac_addr[ 6]+mac_addr[ 7])*(mac_addr[ 8]+mac_addr[ 9]+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[7]=table_6425_pci[((mac_addr[11]+mac_addr[ 7]+mac_addr[ 8])*(           i+mac_addr[10]+mac_addr[11]))%NUMBERINTABLE_6425_PCI];
			final_6425_pci[8]='\0';

			strcpy(final_wepmac_6425_pci, final_6425_pci);
			strcat(final_wepmac_6425_pci, final_6425_pci+3);


			strcpy(flash_field_name, "WEP64_KEY1_1");

			sprintf(final_wepmac1, "flash set %s %02x%02x%02x%02x%02x",
					flash_field_name,
					final_wepmac_6425_pci[0],final_wepmac_6425_pci[1],final_wepmac_6425_pci[4],final_wepmac_6425_pci[6],final_wepmac_6425_pci[7]);
		#else
			#if defined(_BR6225N_) && (defined(_Customer_) || defined(_PCIJP_))
				strcpy(flash_field_name, "WEP128_KEY1_2");
			#else
				strcpy(flash_field_name, "WEP128_KEY1");
			#endif
			sprintf(final_wepmac1, "flash set %s %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				flash_field_name,
				final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
				final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
				final_wepmac[10],final_wepmac[11],final_wepmac[12]);
		#endif
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);

		#if defined(_Customer_) || defined(_PCIJP_)
		sprintf(final_wepmac1, "flash set WLAN_WPA_PSK %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			final_wepmac[0],final_wepmac[2],final_wepmac[4],final_wepmac[6],final_wepmac[8],final_wepmac[10],final_wepmac[12],
			final_wepmac[1],final_wepmac[3],final_wepmac[5],final_wepmac[7],final_wepmac[9],final_wepmac[11]);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);
		#endif
	#if !(defined(_BR6425N_) && defined(_Customer_))
		sprintf(final_wepmac1, "flash set WEP128_KEY1_1 %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11],final_wepmac[12]);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);
	#endif
		#if defined(_BR6225N_) && (defined(_Customer_) || defined(_PCIJP_))
			system("flash set WEP_2 2");
			system("flash set WEP_KEY_TYPE_2  0");
		#elif defined(_BR6425N_) && defined(_Customer_)
			system("flash set WEP_1 1");
			system("flash set WEP_KEY_TYPE_1  0");
		#else
			system("flash set WEP 2");
			system("flash set WEP_KEY_TYPE  0");
		#endif

		//system("flash set IS_RESET_DEFAULT 1");
	}
	else if(argc==3 && !strcmp(argv[2],"wpamac")){
		strcpy(final_wepmac, final);
		#ifndef _GENERATE_ENCRYPTSEED_USEMD5_
		strcat(final_wepmac, final+3);
		#endif
//		printf("%s\n",final_wepmac);

		sprintf(final_wepmac1, 
		#ifdef _Customer_
                "flash set WLAN_WPA_PSK %c%c%c%c%c%c%c%c%c%c%c%c",
                #else
		"flash set WLAN_WPA_PSK %c%c%c%c%c%c%c%c%c%c%c%c%c",
		#endif
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11]
		#ifndef _Customer_
                	,final_wepmac[12]
                #endif
		);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);
		
#ifdef _Customer_
		sprintf(final_wepmac1, "flash set SSID logitec%02d", final[0] % 100);
		system(final_wepmac1);
#endif

		sprintf(final_wepmac1, 
		#ifdef _Customer_
		"flash set INIC_WPA_PSK %c%c%c%c%c%c%c%c%c%c%c%c",
                #else
		"flash set INIC_WPA_PSK %c%c%c%c%c%c%c%c%c%c%c%c%c",
		#endif
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11]
		#ifndef _Customer_
                	,final_wepmac[12]
                #endif
		);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);

#ifdef _Customer_
		sprintf(final_wepmac1, "flash set INIC_SSID logitec11a%02d", final[0] % 100);
		system(final_wepmac1);
#endif
		
		#ifdef _Customer_
		sprintf(final_wepmac1, "flash set USER_PASSWORD %c%c%c%c%c%c%c%c%c%c%c%c",
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11]);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);
		system("if [ \"`pidof webs`\" != \"\" ]; then kill -SIGUSR1 `pidof webs`; fi");
		#endif


		sprintf(final_wepmac1, "flash set WEP128_KEY1_1 %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11],final_wepmac[12]);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);

#ifdef _Customer_
		sprintf(final_wepmac1, "flash set SSID_1 logitec2nd%02d", final[0] % 100);
		system(final_wepmac1);
#endif
		//system("flash set IS_RESET_DEFAULT 1");

	}
#ifdef _WPA_KEY_BY_MAC_INIC_
	else if(argc==3 && !strcmp(argv[2],"wpamacinic")){
		strcpy(final_wepmac, final);
		#ifndef _GENERATE_ENCRYPTSEED_USEMD5_
		strcat(final_wepmac, final+3);
		#endif
		
		sprintf(final_wepmac1, 
		#ifdef _Customer_
		"flash set INIC_WPA_PSK %c%c%c%c%c%c%c%c%c%c%c%c",
                #else
		"flash set INIC_WPA_PSK %c%c%c%c%c%c%c%c%c%c%c%c%c",
		#endif
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11]
		#ifndef _Customer_
                	,final_wepmac[12]
                #endif
		);
		printf("%s\n",final_wepmac1);
		system(final_wepmac1);

	}
#endif
	else if( argc==3 && !strcmp(argv[2],"pwepmac") ) {
		strcpy(final_wepmac, final);
		strcat(final_wepmac, final+3);

		sprintf(final_wepmac1, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			final_wepmac[0],final_wepmac[1],final_wepmac[2],final_wepmac[3],final_wepmac[4],
			final_wepmac[5],final_wepmac[6],final_wepmac[7],final_wepmac[8],final_wepmac[9],
			final_wepmac[10],final_wepmac[11],final_wepmac[12]);
			printf("%s\n", final_wepmac1);
	}
	else if( argc==3 && !strcmp(argv[2],"pincode") )
	{
		unsigned int temp_mac = (((((mac_addr[6]-'0')*16 + (mac_addr[7]-'0'))*16 + (mac_addr[8]-'0'))*16 + (mac_addr[9]-'0'))*16 + (mac_addr[10]-'0'))*16 + (mac_addr[11]-'0');

		temp_mac = temp_mac % 10000000;

		unsigned int result = temp_mac*10 + ComputeChecksum(temp_mac);

		if(result < 10000000) printf("0");
		if(result <  1000000) printf("0");
		if(result <   100000) printf("0");
		if(result <    10000) printf("0");
		if(result <     1000) printf("0");
		if(result <      100) printf("0");
		if(result <       10) printf("0");
		printf("%d\n", result);

	}
	else
	{
#if defined(_2L_)
		printf("%s%02x\n", final, mac_addr[11]);
#else
		printf("%s\n", final);
#endif
	}
}
