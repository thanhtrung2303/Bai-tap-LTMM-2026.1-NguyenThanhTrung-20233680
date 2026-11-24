#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>


int bang_ip[64] = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};

int bang_ip_nghich[64] = {40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};

int bang_e[48] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};

int bang_p[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};

int bang_pc1[56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};

int bang_pc2[48] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};

int so_bit_dich[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

// SBOX
int SBOX[8][4][16] = {
{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};

void hoan_vi(int *vao, int *bang, int so_bit_ra, int *ra)
{
    for (int i = 0; i < so_bit_ra; i++)
        ra[i] = vao[bang[i] - 1];
}

// XOR 2 day bit 
void xor_bit(int *a, int *b, int n, int *ra)
{
    for (int i = 0; i < n; i++)
        ra[i] = a[i] ^ b[i];
}

// So nguyen 64 bit -> day n bit
void so_sang_bit(uint64_t so, int n, int *bit)
{
    for (int i = 0; i < n; i++)
        bit[i] = (int)((so >> (n - 1 - i)) & 1);
}

// Day n bit -> so nguyen
uint64_t bit_sang_so(int *bit, int n)
{
    uint64_t so = 0;

    for (int i = 0; i < n; i++)
        so = (so << 1) | (uint64_t)bit[i];

    return so;
}

// Dich vong trai mang 28 bit di s bit
void dich_trai(int *mang, int s)
{
    int tam[28];
    for (int i = 0; i < 28; i++)
        tam[i] = mang[(i + s) % 28];
    memcpy(mang, tam, sizeof(tam));
}

void des(const char *ban_ro_hex, const char *khoa_hex)
{
    uint64_t ban_ro = (uint64_t)strtoull(ban_ro_hex, NULL, 16);
    uint64_t khoa = (uint64_t)strtoull(khoa_hex, NULL, 16);
    int bit_ro[64], bit_khoa[64], kb[56], c[28], d[28], b[64], l[32], r[32];
    int cd[56], khoa_con[48], mo_rong[48], er[48], so[32], f[32], r_moi[32];
    int truoc_ip1[64], kq[64];

    // Khoa
    so_sang_bit(khoa, 64, bit_khoa);
    hoan_vi(bit_khoa, bang_pc1, 56, kb);
    memcpy(c, kb, 28 * sizeof(int));
    memcpy(d, kb + 28, 28 * sizeof(int));

    // Ban ro
    so_sang_bit(ban_ro, 64, bit_ro);
    hoan_vi(bit_ro, bang_ip, 64, b);
    memcpy(l, b, 32 * sizeof(int));
    memcpy(r, b + 32, 32 * sizeof(int));
    printf("L0: %08" PRIx64 " R0: %08" PRIx64 "\n", bit_sang_so(l, 32), bit_sang_so(r, 32));

    for (int vong = 0; vong < 16; vong++) {
        // Tao khoa con
        dich_trai(c, so_bit_dich[vong]);
        dich_trai(d, so_bit_dich[vong]);
        memcpy(cd, c, 28 * sizeof(int));
        memcpy(cd + 28, d, 28 * sizeof(int));
        hoan_vi(cd, bang_pc2, 48, khoa_con);

        // Ham f: E(R) xor K -> S-box -> P
        hoan_vi(r, bang_e, 48, mo_rong);
        xor_bit(mo_rong, khoa_con, 48, er);
        for (int j = 0; j < 8; j++) {
            int *nhom = er + 6 * j;
            int hang = nhom[0] * 2 + nhom[5];
            int cot = nhom[1] * 8 + nhom[2] * 4 + nhom[3] * 2 + nhom[4];
            int gia_tri = SBOX[j][hang][cot];
            for (int k = 0; k < 4; k++)
                so[4 * j + k] = (gia_tri >> (3 - k)) & 1;
        }
        hoan_vi(so, bang_p, 32, f);

        // L_i = R_(i-1), R_i = L_(i-1) xor f 
        xor_bit(l, f, 32, r_moi);
        memcpy(l, r, 32 * sizeof(int));
        memcpy(r, r_moi, 32 * sizeof(int));

        printf("vong %d K:%012" PRIx64 " L:%08" PRIx64 " R:%08" PRIx64 "\n",
                vong + 1,
                bit_sang_so(khoa_con, 48),
                bit_sang_so(l, 32),
                bit_sang_so(r, 32));
    }

    // Hoan vi cuoi
    memcpy(truoc_ip1, r, 32 * sizeof(int));
    memcpy(truoc_ip1 + 32, l, 32 * sizeof(int));
    hoan_vi(truoc_ip1, bang_ip_nghich, 64, kq);
    printf("out: %016" PRIx64 "\n", bit_sang_so(kq, 64));
}

int main(void)
{
    printf("DES 1:\n");
    des("02468aceeca86420", "0f1571c947d9e859");
    printf("DES 2:\n");
    des("0123456789abcdef", "133457799bbcdff1");
    printf("DES 3:\n");
    des("0000000000000011", "0000000000000000");
    return 0;
}
