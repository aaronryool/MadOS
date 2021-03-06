////////////////////////////////////////////////////////////////////////////////
// THE SCOTCH-WARE LICENSE (Revision 0):
// <aaronryool/gmail.com> wrote this file. As long as you retain this notice you
// can do whatever you want with this stuff. If we meet some day, and you think
// this stuff is worth it, you can buy me a shot of scotch in return
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <video.h>

const char* const sys_errlist[] = {
    "out of memory!!!",
    "bad limit...",
    "not implemented",
    "too many disks",
    NULL
};


unsigned char (*getch) (void);

int fputch(FILE* stream, char c)
{
    return write((int) stream, &c, 1);
}

int fputc(int c, FILE* stream)
{
    return write((int) stream, &c, 1);
}

size_t fwrite(const void *array, size_t size, size_t count, FILE *stream)
{
    // flesh out later
    return write((int) stream, array, count*size);
}


int puts(const char* s)
{
    if(s == NULL) return -1;
    int ret = write(1, s, strlen(s));
    write((int) stdout, "\n", 1);
    return ret;
}


char* __printf_f_s(char* p)
{
    return p;
}
char* __printf_f_c(char p)
{
    char* o = malloc(2);
    o[0] = p;
    o[1] = 0;
    free(o);
    return o;
}

char* __printf_f_i(void* p)
{
    char* ret = itoa((int) p, 10, true);
    free(ret);
    return ret;
}

char* __printf_f_d(void* p)
{
    return __printf_f_i(p);
}
char* __printf_f_u(void* p)
{
    char* ret = itoa((int) p, 10, false);
    free(ret);
    return ret;
}
char* __printf_f_x(void* p)
{
    char* ret = itoa((unsigned) p, 16, false);
    free(ret);
    return ret;
}
char* __printf_f_b(void* p)
{
    char* ret = itoa((unsigned) p, 2, false);
    free(ret);
    return ret;
}

char format_flags[25] = "sciduxb";
char* (*printf_fun_array[25]) (void*) = {
        (typeof(*printf_fun_array)) __printf_f_s,
        (typeof(*printf_fun_array)) __printf_f_c,
        __printf_f_i,        
        __printf_f_d,
        __printf_f_u,
        __printf_f_x,
        __printf_f_b,
        NULL
};

int register_printf_function(int spec, printf_function handler)
{
    int size = strlen(format_flags);
    if(size >= 128)
        return -1;
    printf_fun_array[size] = handler;
    format_flags[size] = (char) spec;
    
    printf_fun_array[size++] = NULL;
    format_flags[size] = null;

    return 0;
}

int sprintf(char* str, const char* __restrict format, ...)
{
    va_list argp;
    void* p;
    int i, c, ccount = 0;
    va_start(argp, format);
    for(i = 0;format[i] != null;i++)
    {
        switch(format[i])
        {
            case '%':
                i++;
                if(cpos(format_flags, format[i]) != -1)
                {
                    p = (void*) va_arg(argp, void*);
                    char* out = printf_fun_array[cpos(format_flags, format[i])](p);
                    for(c = 0;out[c] != null;c++, ccount++)
                        str[ccount] = out[c];
                }
                else
                {
                    str[ccount] = format[i];
                    ccount++;
                }
                continue;
            default:
                str[ccount] = format[i];
                ccount++;
                continue;
        }
    }
    va_end(argp);
    str[ccount] = null;
    return ccount;
}


int fprintf(FILE* stream, const char* __restrict format, ...)
{
    va_list argp;
    void* p;
    int i, ccount = 0;
    va_start(argp, format);
    for(i = 0;format[i] != null;i++)
    {
        switch(format[i])
        {
            case '%':
                i++;
                if(cpos(format_flags, format[i]) != -1)
                {
                    p = (void*) va_arg(argp, void*);
                    char* out = printf_fun_array[cpos(format_flags, format[i])](p);
                    ccount += fwrite(out, sizeof(char), strlen(out), stream);
                }
                else
                {
                    fputch(stream, format[i]);
                    ccount++;
                }
                continue;
            default:
                fputch(stream, format[i]);
                ccount++;
                continue;
        }
    }
    va_end(argp);
    return ccount;
}

extern void halt(void);
void panic(const char* s, unsigned int e)
{
    write((int) stderr, "\n\n", 2);
    write((int) stderr, s, strlen(s));
    write((int) stderr, ": ", 2);
    write((int) stderr, sys_errlist[e], strlen(sys_errlist[e]));
    putchar(' ');
    video_setcolor(MAKE_COLOR(COLOR_WHITE, COLOR_RED));
    write((int) stderr," Kernel Panic ", 15);
    video_setcolor(MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}


