/* According to Serial Flasher Protocol Specification - version 1 */
#define S_ACK 0x06
#define S_NAK 0x15
#define S_CMD_NOP		0x00	/* No operation					*/
#define S_CMD_Q_IFACE		0x01	/* Query interface version			*/
#define S_CMD_Q_CMDMAP		0x02	/* Query supported commands bitmap		*/
#define S_CMD_Q_PGMNAME		0x03	/* Query programmer name			*/
#define S_CMD_Q_SERBUF		0x04	/* Query Serial Buffer Size			*/
#define S_CMD_Q_BUSTYPE		0x05	/* Query supported bustypes			*/
#define S_CMD_Q_CHIPSIZE	0x06	/* Query supported chipsize (2^n format)	*/
#define S_CMD_Q_OPBUF		0x07	/* Query operation buffer size			*/
#define S_CMD_Q_WRNMAXLEN	0x08	/* Query Write to opbuf: Write-N maximum length */
#define S_CMD_R_BYTE		0x09	/* Read a single byte				*/
#define S_CMD_R_NBYTES		0x0A	/* Read n bytes					*/
#define S_CMD_O_INIT		0x0B	/* Initialize operation buffer			*/
#define S_CMD_O_WRITEB		0x0C	/* Write opbuf: Write byte with address		*/
#define S_CMD_O_WRITEN		0x0D	/* Write to opbuf: Write-N			*/
#define S_CMD_O_DELAY		0x0E	/* Write opbuf: udelay				*/
#define S_CMD_O_EXEC		0x0F	/* Execute operation buffer			*/
#define S_CMD_SYNCNOP		0x10	/* Special no-operation that returns NAK+ACK	*/
#define S_CMD_Q_RDNMAXLEN	0x11	/* Query read-n maximum length			*/
#define S_CMD_S_BUSTYPE		0x12	/* Set used bustype(s).				*/
#define S_CMD_O_SPIOP		0x13	/* Perform SPI operation.			*/
#define S_CMD_S_SPI_FREQ	0x14	/* Set SPI clock frequency			*/
#define S_CMD_S_PIN_STATE	0x15	/* Enable/disable output drivers		*/
#define S_CMD_NUM	        0x16	/* Total cmd number		*/

#define SERPROG_VERSION (0x01)
#define SERPROG_CMD_MAP ( \
  (1 << S_CMD_NOP)       | \
  (1 << S_CMD_Q_IFACE)   | \
  (1 << S_CMD_Q_CMDMAP)  | \
  (1 << S_CMD_Q_PGMNAME) | \
  (1 << S_CMD_Q_BUSTYPE) | \
  (1 << S_CMD_Q_RDNMAXLEN) | \
  (1 << S_CMD_Q_WRNMAXLEN) | \
  (1 << S_CMD_SYNCNOP)   | \
  (1 << S_CMD_S_BUSTYPE)   	| \
  (1 << S_CMD_O_SPIOP)   	| \
  (1 << S_CMD_S_SPI_FREQ)	\
)

#define SERPROG_NAME ("pico serprog")

#define SERPROG_BUFFER_SIZE (0xffff)	//64K

enum chipbustype {
	BUS_NONE	= 0,
	BUS_PARALLEL	= 1 << 0,
	BUS_LPC		= 1 << 1,
	BUS_FWH		= 1 << 2,
	BUS_SPI		= 1 << 3,
	BUS_PROG	= 1 << 4,
	BUS_NONSPI	= BUS_PARALLEL | BUS_LPC | BUS_FWH,
};

#define SERPROG_BUS_TYPE BUS_SPI
#define SERPROG_MAX_WRITE_N (256)
#define SERPROG_MAX_READ_N (64 * 1024)
#define SERPROG_MAX_OP_N (64 * 1024)

typedef struct serprog_answer
{
    uint8_t cmd;
	uint8_t ack;

    uint32_t parmlen;
    void *params;

    uint32_t retlen;
    void *retparms;

    int (*answer)(uint8_t cmd, uint8_t ack, uint32_t parmlen,
			uint8_t *params, uint32_t retlen, void *retparms);

}serprog_answer_t;

/* 定义返回ACK的响应 */
#define ACK_ANSWER_DECLARE(_cmd, _parmlen, _params, _retlen, _retparms, _answer) \
	{\
		.cmd = _cmd, \
		.ack = S_ACK,	\
		.parmlen = _parmlen, \
		.params = _params, \
		.retlen = _retlen, \
		.retparms = _retparms, \
		.answer = _answer, \
	}

/* 定义返回NO ACK的响应 */
#define NAK_ANSWER_DECLARE(_cmd, _parmlen, _params, _retlen, _retparms, _answer) \
	{\
		.cmd = _cmd, \
		.ack = S_NAK,	\
		.parmlen = _parmlen, \
		.params = _params, \
		.retlen = _retlen, \
		.retparms = _retparms, \
		.answer = _answer, \
	}

int handle_cmd(uint8_t cmd);