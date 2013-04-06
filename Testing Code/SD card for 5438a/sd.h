#ifndef SD_H_
#define SD_H_

#include <msp430f5438a.h>
#include "sdcomm_spi.h"
#include "types.h"

#define SD_BLOCKSIZE 512
#define SD_BLOCKSIZE_NBITS 9
/* User functions */
int sd_card_present();
int sd_initialize(sd_context_t *sdc);
int sd_read_block (sd_context_t *sdc, u32 blockaddr, unsigned char *data);
int sd_write_block (sd_context_t *sdc, u32 blockaddr, unsigned char *data);
void sd_wait_notbusy (sd_context_t *sdc);
/* Internal functions, used for SD card communications. */
void sd_packarg(unsigned char *argument, u32 value);
int sd_set_blocklen (sd_context_t *sdc, u32 length);
int sd_send_command(sd_context_t *sdc,
		unsigned char cmd, unsigned char response_type,
		unsigned char *response, unsigned char *argument);
void sd_delay(char number);



#endif /* SD_H_ */
