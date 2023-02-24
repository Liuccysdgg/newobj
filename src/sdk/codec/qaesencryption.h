#ifndef QAESENCRYPTION_H
#define QAESENCRYPTION_H
#include "public/buffer.h"
#include "public/define.h"

#if LIB_QT == 1
#ifdef __linux__
#ifndef __LP64__
#define do_rdtsc _do_rdtsc
#endif
#endif

class QAESEncryption
{
public:
    enum Aes {
        AES_128,
        AES_192,
        AES_256
    };

    enum Mode {
        ECB,
        CBC,
        CFB,
        OFB
    };

    enum Padding {
      ZERO,
      PKCS7,
      ISO
    };

    static QByteArray Crypt(QAESEncryption::Aes level, QAESEncryption::Mode mode, const QByteArray &rawText, const QByteArray &key,
                            const QByteArray &iv = QByteArray(), QAESEncryption::Padding padding = QAESEncryption::ISO);
    static QByteArray Decrypt(QAESEncryption::Aes level, QAESEncryption::Mode mode, const QByteArray &rawText, const QByteArray &key,
                              const QByteArray &iv = QByteArray(), QAESEncryption::Padding padding = QAESEncryption::ISO);
    static QByteArray ExpandKey(QAESEncryption::Aes level, QAESEncryption::Mode mode, const QByteArray &key);
    static QByteArray RemovePadding(const QByteArray &rawText, QAESEncryption::Padding padding = QAESEncryption::ISO);

    QAESEncryption(QAESEncryption::Aes level, QAESEncryption::Mode mode,
                   QAESEncryption::Padding padding = QAESEncryption::ISO);

    QByteArray encode(const QByteArray &rawText, const QByteArray &key, const QByteArray &iv = QByteArray());
    QByteArray decode(const QByteArray &rawText, const QByteArray &key, const QByteArray &iv = QByteArray());
    QByteArray removePadding(const QByteArray &rawText);
    QByteArray expandKey(const QByteArray &key);

    QByteArray printArray(uchar *arr, int size);

private:
    int m_nb;
    int m_blocklen;
    int m_level;
    int m_mode;
    int m_nk;
    int m_keyLen;
    int m_nr;
    int m_expandedKey;
    int m_padding;
    bool m_aesNIAvailable;
    QByteArray* m_state;

    struct AES256{
        AES256()
        {
            nk = 8;
            keylen = 32;
            nr = 14;
            expandedKey = 240;
        }
        int nk;
        int keylen;
        int nr;
        int expandedKey;
    };

    struct AES192{
        AES192()
        {
            nk = 6;
            keylen = 24;
            nr = 12;
            expandedKey = 209;
        }
        int nk;
        int keylen;
        int nr;
        int expandedKey;
    };

    struct AES128{
        AES128()
        {
            nk = 4;
            keylen = 16;
            nr = 10;
            expandedKey = 176;
        }
        int nk;
        int keylen;
        int nr;
        int expandedKey;
    };

    quint8 getSBoxValue(quint8 num){return sbox[num];}
    quint8 getSBoxInvert(quint8 num){return rsbox[num];}

    void addRoundKey(const quint8 round, const QByteArray &expKey);
    void subBytes();
    void shiftRows();
    void mixColumns();
    void invMixColumns();
    void invSubBytes();
    void invShiftRows();
    QByteArray getPadding(int currSize, int alignment);
    QByteArray cipher(const QByteArray &expKey, const QByteArray &in);
    QByteArray invCipher(const QByteArray &expKey, const QByteArray &in);
    QByteArray byteXor(const QByteArray &a, const QByteArray &b);

    quint8 sbox[256];

    quint8 rsbox[256];

    // The round constant word array, Rcon[i], contains the values given by
    // x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
    // Only the first 14 elements are needed
    quint8 Rcon[14];
};
#endif
#endif // QAESENCRYPTION_H
