#include <mkdebug.h>
#include <mkinc.h>

#define MK_RTOS_GLOABLS 1

/**
 * @brief 断言处理函数
 */
void _MK_ATTRIBUTE(__noreturn__)
    __mk_assert_func(const char *_file, int _line, const char *_func_name, const char *_error) {
    mkprintk("file:%s,line:%d,fun:%s,err:%s\r\n", _file, _line, _func_name, _error);
}

#if MK_USE_QMENU_DEBUG
#include <stdarg.h>

void mk_init_log(void) {
    
}

mk_weak volatile mk_uint32_t *const UART0DR = (mk_uint32_t *)0x4000C000;
mk_weak char mk_putc(mk_uint8_t *ch) {
    *UART0DR = *ch;
    return *ch;
}

const char hex_asc_table[16] =
    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
const char upper_hex_asc_table[16] =
    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static int is_dec_asc(char ch) {
    mk_uint32_t i;
    for (i = 0; i < 10; i++) {
        if (ch == hex_asc_table[i])
            return 1;
    }

    return 0;
}

static int is_asc(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

static int is_hex_asc(char ch) {
    return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
}

static int printf_char(char ch) {
    mk_uint8_t c1 = (mk_uint8_t)'\r';
    mk_uint8_t c2 = 0;

    c2 = (mk_uint8_t)ch;

    mk_putc(&c2);

    if (((mk_uint8_t)'\n') == c2) {
        mk_putc(&c1);
    }

    return 0;
}

static int printf_str(char *str) {
    while (str && (*str != (char)'\0')) {
        printf_char(*str);
        str++;
    }

    return 0;
}

static int printf_hex(mk_uint32_t val, mk_uint32_t width) {
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

static int printf_hex_upper(mk_uint32_t val, mk_uint32_t width) {
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = upper_hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

const mk_uint32_t hex_weight_value_table[] =
    {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

static int printf_dec(mk_uint32_t val) {
    mk_uint32_t i = 0;
    mk_uint32_t tmp = 1, tmp_w = 1;
    char asc = 0;

    /* Figure out the digitals */
    while (1) {
        tmp = (tmp << 3) + (tmp << 1); // tmp *= 10;
        i++;
        if (tmp > val) {
            break;
        }
    }

    if (i > 8) {
        return -1;
    }

    while (i > 0) {
        if (val >= ((hex_weight_value_table[i - 1] << 3) + hex_weight_value_table[i - 1])) { //<=9xxx
            tmp = 9;
            tmp_w = (hex_weight_value_table[i - 1] << 3) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 3)) { // 8xxx
            tmp = 8;
            tmp_w = hex_weight_value_table[i - 1] << 3;
        } else if (val >= ((hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1])) {
            tmp = 7;
            tmp_w =
                (hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1];
        } else if (val >= ((hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1))) {
            tmp = 6;
            tmp_w = (hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1);
        } else if (val >= ((hex_weight_value_table[i - 1] << 2) + hex_weight_value_table[i - 1])) {
            tmp = 5;
            tmp_w = (hex_weight_value_table[i - 1] << 2) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 2)) {
            tmp = 4;
            tmp_w = hex_weight_value_table[i - 1] << 2;
        } else if (val >= ((hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1])) {
            tmp = 3;
            tmp_w = (hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 1)) {
            tmp = 2;
            tmp_w = hex_weight_value_table[i - 1] << 1;
        } else if (val >= (hex_weight_value_table[i - 1])) {
            tmp = 1;
            tmp_w = hex_weight_value_table[i - 1];
        } else {
            tmp = 0;
            tmp_w = 0;
        }

        asc = hex_asc_table[tmp];
        printf_char(asc);
        i--;

        val -= tmp_w;
    }

    return 0;
}

void mkprintk(const char *fmt, ...) {
    char c;
    mk_uint32_t width = 0;
    va_list argptr;

    va_start(argptr, fmt);
    do {
        c = *fmt;
        if (c != '%') {
            printf_char(c);
        } else {
            while (1) {
                c = *++fmt;
                if ((c == 'd') || (c == 'x') || (c == 'X') || (c == 's') || (c == 'c')) {
                    if ((c == 'x') || (c == 'X')) {
                        if (*(fmt - 1) == '%')
                            width = 8;
                        else
                            width = *(fmt - 1) - '0';
                    }
                    break;
                }
            }

            switch (c) {
                case 'd':
                    printf_dec(va_arg(argptr, int));
                    break;
                case 'x':
                    printf_hex((va_arg(argptr, int)), width);
                    break;
                case 'X':
                    printf_hex_upper((va_arg(argptr, int)), width);
                    break;
                case 's':
                    printf_str(va_arg(argptr, char *));
                    break;
                case 'c':
                    printf_char(va_arg(argptr, int));
                    break;
                default:
                    break;
            }
        }
        ++fmt;
    } while (*fmt != '\0');

    va_end(argptr);
}

#else
MK_RTOS_EXT void mk_rtos_putc(unsigned char ch);
MK_RTOS_EXT void mk_rtos_console_init(void);

void mk_init_log(void) {
    mk_rtos_console_init();
}

void mkprintk(const char *fmt, ...) {
    mk_base_t i;
    va_list args;
    mk_size_t length;
    static char log_buf[MK_DEBUG_BUFF_SIZE];
    va_start(args, fmt);
    length = vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, args);
    va_end(args);
    if (length > MK_DEBUG_BUFF_SIZE - 1)
        length = MK_DEBUG_BUFF_SIZE - 1;
    for (i = 0; i < length; i++) {
        mk_rtos_putc(log_buf[i]);
    }
}

#endif