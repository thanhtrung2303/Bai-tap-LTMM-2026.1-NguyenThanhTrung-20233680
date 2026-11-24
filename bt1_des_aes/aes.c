#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// Bang SBOX
uint8_t SBOX[256] = {
0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// Hang so vong Rcon
uint8_t Rcon[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

// Nhan voi x trong GF(2^8)
uint8_t nhan_x(uint8_t a)
{
    return (a & 0x80) ? (uint8_t)((a << 1) ^ 0x1b) : (uint8_t)(a << 1);
}

// Nhan 2 ptu trong GF(2^8)
uint8_t nhan_gf(uint8_t a, uint8_t b)
{
    uint8_t kq = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) kq ^= a;
        a = nhan_x(a);
        b >>= 1;
    }
    return kq;
}

// Đoi chuoi hex sang 16 byte
void hex_sang_byte(const char *hex, uint8_t *ra)
{
    char cnt[33];
    int len = (int)strlen(hex);
    int so_so_0 = 32 - len;
    for (int i = 0; i < so_so_0; i++) cnt[i] = '0';
    strcpy(cnt + so_so_0, hex);
    for (int i = 0; i < 16; i++) {
        char hai_ky_tu[3] = {cnt[2 * i], cnt[2 * i + 1], 0};
        ra[i] = (uint8_t)strtoul(hai_ky_tu, NULL, 16);
    }
}

// In trang thai theo cot
void in_trang_thai(uint8_t trang_thai[4][4])
{
    for (int cot = 0; cot < 4; cot++)
        for (int hang = 0; hang < 4; hang++)
            printf("%02x", trang_thai[hang][cot]);
}

void aes(const char *ban_ro_hex, const char *khoa_hex)
{
    uint8_t khoa[16], ban_ro[16], w[44][4], trang_thai[4][4];

    hex_sang_byte(khoa_hex, khoa);
    hex_sang_byte(ban_ro_hex, ban_ro);

    for (int i = 0; i < 4; i++)
        memcpy(w[i], khoa + 4 * i, 4);
    for (int i = 4; i < 44; i++) {
        uint8_t t[4];
        memcpy(t, w[i - 1], 4);
        if (i % 4 == 0) {
            uint8_t dau = t[0];
            t[0] = SBOX[t[1]];
            t[1] = SBOX[t[2]];
            t[2] = SBOX[t[3]];
            t[3] = SBOX[dau];
            t[0] ^= Rcon[i / 4];
        }
        for (int j = 0; j < 4; j++)
            w[i][j] = w[i - 4][j] ^ t[j];
    }

    // Nap trang thai va AddRoundKey voi khoa vong 0
    for (int hang = 0; hang < 4; hang++)
        for (int cot = 0; cot < 4; cot++)
            trang_thai[hang][cot] = ban_ro[4 * cot + hang] ^ w[cot][hang];

    for (int vong = 1; vong <= 10; vong++) {
        // SubBytes
        for (int hang = 0; hang < 4; hang++)
            for (int cot = 0; cot < 4; cot++)
                trang_thai[hang][cot] = SBOX[trang_thai[hang][cot]];
        if (vong == 1) {
            printf("SubBytes: ");
            in_trang_thai(trang_thai);
            printf("\n");
        }

        // ShiftRows
        for (int hang = 1; hang < 4; hang++) {
            uint8_t tam[4];
            for (int cot = 0; cot < 4; cot++)
                tam[cot] = trang_thai[hang][(cot + hang) % 4];
            memcpy(trang_thai[hang], tam, 4);
        }
        if (vong == 1) {
            printf("ShiftRows: ");
            in_trang_thai(trang_thai);
            printf("\n");
        }

        // MixColumns
        if (vong < 10) {
            for (int cot = 0; cot < 4; cot++) {
                uint8_t a0 = trang_thai[0][cot], a1 = trang_thai[1][cot];
                uint8_t a2 = trang_thai[2][cot], a3 = trang_thai[3][cot];
                trang_thai[0][cot] = nhan_gf(2, a0) ^ nhan_gf(3, a1) ^ a2 ^ a3;
                trang_thai[1][cot] = a0 ^ nhan_gf(2, a1) ^ nhan_gf(3, a2) ^ a3;
                trang_thai[2][cot] = a0 ^ a1 ^ nhan_gf(2, a2) ^ nhan_gf(3, a3);
                trang_thai[3][cot] = nhan_gf(3, a0) ^ a1 ^ a2 ^ nhan_gf(2, a3);
            }
        }
        if (vong == 1 && vong < 10) {
            printf("MixColumns: ");
            in_trang_thai(trang_thai);
            printf("\n");
        }

        // AddRoundKey
        for (int cot = 0; cot < 4; cot++)
            for (int hang = 0; hang < 4; hang++)
                trang_thai[hang][cot] ^= w[4 * vong + cot][hang];
        if (vong == 1) {
            printf("AddRoundKey: ");
            in_trang_thai(trang_thai);
            printf("\n");
        }
        printf("vong %d K:", vong);
        for (int cot = 0; cot < 4; cot++)
            for (int hang = 0; hang < 4; hang++)
                printf("%02x", w[4 * vong + cot][hang]);
        printf(" st:");
        in_trang_thai(trang_thai);
        printf("\n");
    }
    printf("out: ");
    in_trang_thai(trang_thai);
    printf("\n");
}

int main(void)
{
    printf("AES:\n");
    aes("0000000000000011", "0000000000000000");
    return 0;
}
