/*
 * File:   Error.h
 * Author: jony
 *
 * Created on 12 de Junho de 2013, 22:51
 */

#ifndef ERROR_H
#define	ERROR_H

class Erro {
    public:
    static const int LACK_OF_MEMORY=1;
    static const int FILE_NOT_FOUND=2;
    static const int BROKEN_FILE=3;
    static const int CODEC_SUPERIOR=4;
    static const int UNKNOW_FILE=5;
    static const int UNEXPECTED_END_OF_LINE=6;
    static const int UNEXPECTED_END_OF_FILE=7;
};

int showErr(int msg);
int showErr(int msg, int line, const char token[50]);
int get_errors();




#endif	/* ERROR_H */

