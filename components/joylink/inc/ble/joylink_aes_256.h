#ifndef __JOYLINK_AES_256_H__
#define __JOYLINK_AES_256_H__

void key_expansion(const uint8_t *key, uint8_t *w);
void cipher(uint8_t *in,  uint8_t *w);
void inv_cipher(uint8_t *in, uint8_t *w);

#endif
