#include "stage8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int g_artist_count = 0;
Artist g_artists[MAX_ARTISTS];

static EncryptionAlgorithm g_algo = ENC_XOR;
static unsigned char g_key = 0;

void strip_newline(char *s) {
    char *p = strchr(s, '\n');
    if (p) *p = 0;
}

void pause() {
    printf("Press Enter to continue...");
    getchar();
}

// --- Encryption Functions ---

void encrypt_xor(char *dst, const char *src, unsigned char key) {
    for (int i = 0; src[i]; ++i) dst[i] = src[i] ^ key;
    dst[strlen(src)] = 0;
}
void decrypt_xor(char *dst, const char *src, unsigned char key) {
    encrypt_xor(dst, src, key);
}
void encrypt_or(char *dst, const char *src, unsigned char key) {
    for (int i = 0; src[i]; ++i) dst[i] = src[i] | key;
    dst[strlen(src)] = 0;
}
void decrypt_or(char *dst, const char *src, unsigned char key) {
    for (int i = 0; src[i]; ++i) dst[i] = src[i] & (~key);
    dst[strlen(src)] = 0;
}
void encrypt_and(char *dst, const char *src, unsigned char key) {
    for (int i = 0; src[i]; ++i) dst[i] = src[i] & key;
    dst[strlen(src)] = 0;
}
void decrypt_and(char *dst, const char *src, unsigned char key) {
    // Not perfectly reversible for all inputs, but for demo:
    // Can't reconstruct original if lossy, just display bytes.
    for (int i = 0; src[i]; ++i) dst[i] = src[i]; // returns as is
    dst[strlen(src)] = 0;
}
void encrypt_shl(char *dst, const char *src, unsigned char key) {
    unsigned char shift = key % 8;
    for (int i = 0; src[i]; ++i) dst[i] = src[i] << shift;
    dst[strlen(src)] = 0;
}
void decrypt_shl(char *dst, const char *src, unsigned char key) {
    unsigned char shift = key % 8;
    for (int i = 0; src[i]; ++i) dst[i] = (unsigned char)src[i] >> shift;
    dst[strlen(src)] = 0;
}
void encrypt_shr(char *dst, const char *src, unsigned char key) {
    unsigned char shift = key % 8;
    for (int i = 0; src[i]; ++i) dst[i] = (unsigned char)src[i] >> shift;
    dst[strlen(src)] = 0;
}
void decrypt_shr(char *dst, const char *src, unsigned char key) {
    unsigned char shift = key % 8;
    for (int i = 0; src[i]; ++i) dst[i] = src[i] << shift;
    dst[strlen(src)] = 0;
}

// For selecting algorithm
void show_algo_menu() {
    printf("Select Encryption Algorithm:\n");
    printf(" 1. XOR\n 2. OR\n 3. AND\n 4. Shift Left\n 5. Shift Right\nSelect: ");
}
void set_encryption_algo() {
    char buf[16];
    show_algo_menu();
    fgets(buf, sizeof(buf), stdin);
    int choice = atoi(buf);
    if (choice >= 1 && choice <= 5)
        g_algo = (EncryptionAlgorithm)choice;
    else
        g_algo = ENC_XOR;
}

void get_encryption_key() {
    char buf[16];
    printf("Enter 1-byte Encryption Key (0~255): ");
    fgets(buf, sizeof(buf), stdin);
    g_key = (unsigned char)atoi(buf);
}

void encrypt_field(char *dst, const char *src) {
    switch (g_algo) {
    case ENC_XOR: encrypt_xor(dst, src, g_key); break;
    case ENC_OR:  encrypt_or(dst, src, g_key); break;
    case ENC_AND: encrypt_and(dst, src, g_key); break;
    case ENC_SHL: encrypt_shl(dst, src, g_key); break;
    case ENC_SHR: encrypt_shr(dst, src, g_key); break;
    }
}
void decrypt_field(char *dst, const char *src) {
    switch (g_algo) {
    case ENC_XOR: decrypt_xor(dst, src, g_key); break;
    case ENC_OR:  decrypt_or(dst, src, g_key); break;
    case ENC_AND: decrypt_and(dst, src, g_key); break;
    case ENC_SHL: decrypt_shl(dst, src, g_key); break;
    case ENC_SHR: decrypt_shr(dst, src, g_key); break;
    }
}

// --- File I/O ---

#define ARTIST_FILE "artist_data.bin"

void save_artists_to_file() {
    FILE *fp = fopen(ARTIST_FILE, "wb");
    if (!fp) {
        printf("File write error.\n");
        return;
    }
    fwrite(&g_artist_count, sizeof(int), 1, fp);
    fwrite(g_artists, sizeof(Artist), g_artist_count, fp);
    fclose(fp);
}

void load_artists_from_file() {
    FILE *fp = fopen(ARTIST_FILE, "rb");
    if (!fp) {
        g_artist_count = 0;
        return;
    }
    fread(&g_artist_count, sizeof(int), 1, fp);
    fread(g_artists, sizeof(Artist), g_artist_count, fp);
    fclose(fp);
}

// --- Artist Input ---

void input_artist(Artist *a) {
    char buf[256];
    printf("Name: "); fgets(a->name, MAX_NAME_LEN, stdin); strip_newline(a->name);
    printf("Nickname: "); fgets(a->nickname, MAX_NICKNAME_LEN, stdin); strip_newline(a->nickname);
    printf("Date of Birth (YYYY-MM-DD): "); fgets(a->dob, MAX_DOB_LEN, stdin); strip_newline(a->dob);
    printf("Gender: "); fgets(a->gender, MAX_GENDER_LEN, stdin); strip_newline(a->gender);
    printf("Education: "); fgets(a->education, MAX_EDU_LEN, stdin); strip_newline(a->education);

    printf("Phone Number: "); fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    encrypt_field(a->phone_enc, buf);

    printf("Email: "); fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    encrypt_field(a->email_enc, buf);

    printf("Nationality: "); fgets(a->nationality, MAX_NATION_LEN, stdin); strip_newline(a->nationality);
    printf("Height (cm): "); fgets(buf, sizeof(buf), stdin); a->height = atof(buf);
    printf("Weight (kg): "); fgets(buf, sizeof(buf), stdin); a->weight = atof(buf);
    printf("Blood Type: "); fgets(a->blood, MAX_BLOOD_LEN, stdin); strip_newline(a->blood);

    printf("Allergies: "); fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    encrypt_field(a->allergy_enc, buf);

    printf("SNS: "); fgets(a->sns, MAX_SNS_LEN, stdin); strip_newline(a->sns);
}

// --- Artist Display ---

void print_artist_list() {
    printf("\n%-3s | %-20s | %-12s\n", "No", "Name", "Nickname");
    printf("-------------------------------\n");
    for (int i = 0; i < g_artist_count; ++i)
        printf("%-3d | %-20s | %-12s\n", i+1, g_artists[i].name, g_artists[i].nickname);
}

// Show artist detail (optionally decrypted)
void print_artist_detail(const Artist *a, int decrypt) {
    char phone[MAX_PHONE_LEN] = {0}, email[MAX_EMAIL_LEN] = {0}, allergy[MAX_ALLERGY_LEN] = {0};
    if (decrypt) {
        decrypt_field(phone, a->phone_enc);
        decrypt_field(email, a->email_enc);
        decrypt_field(allergy, a->allergy_enc);
    }
    printf("\n----- Artist Info -----\n");
    printf("Name: %s\n", a->name);
    printf("Nickname: %s\n", a->nickname);
    printf("Date of Birth: %s\n", a->dob);
    printf("Gender: %s\n", a->gender);
    printf("Education: %s\n", a->education);
    printf("Phone Number: %s\n", decrypt ? phone : "(Encrypted)");
    printf("Email: %s\n", decrypt ? email : "(Encrypted)");
    printf("Nationality: %s\n", a->nationality);
    printf("Height: %.2f\n", a->height);
    printf("Weight: %.2f\n", a->weight);
    printf("Blood Type: %s\n", a->blood);
    printf("Allergies: %s\n", decrypt ? allergy : "(Encrypted)");
    printf("SNS: %s\n", a->sns);
}

// --- Main Protect Functionality ---

void protectMyData() {
    load_artists_from_file();

    char buf[16];
    while (1) {
        printf("\n=== Data Protection Menu ===\n");
        printf("1. Input Artists\n2. View Artists\n3. Exit\nSelect: ");
        fgets(buf, sizeof(buf), stdin);
        int sel = atoi(buf);
        if (sel == 1) {
            set_encryption_algo();
            get_encryption_key();
            printf("Enter number of artists (max %d): ", MAX_ARTISTS);
            fgets(buf, sizeof(buf), stdin);
            int n = atoi(buf);
            if (n < 1) n = 1;
            if (n > MAX_ARTISTS) n = MAX_ARTISTS;
            g_artist_count = n;
            for (int i = 0; i < n; ++i) {
                printf("\n-- Artist #%d --\n", i+1);
                input_artist(&g_artists[i]);
            }
            save_artists_to_file();
            printf("Artists saved (with encrypted fields).\n");
            pause();
        } else if (sel == 2) {
            if (g_artist_count == 0) {
                printf("No artists found.\n");
                pause();
                continue;
            }
            print_artist_list();
            printf("\nSelect by No (0 to return): ");
            fgets(buf, sizeof(buf), stdin);
            int idx = atoi(buf) - 1;
            if (idx < 0 || idx >= g_artist_count) continue;
            print_artist_detail(&g_artists[idx], 0);
            printf("\nDecrypt sensitive info? (y/n): ");
            fgets(buf, sizeof(buf), stdin);
            if (tolower(buf[0]) == 'y') {
                set_encryption_algo();
                get_encryption_key();
                print_artist_detail(&g_artists[idx], 1);
            }
            pause();
        } else if (sel == 3) {
            break;
        } else {
            printf("Invalid selection.\n");
        }
    }
}
