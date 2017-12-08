/****************************************************************************

 ****************************************************************************/
#include "WWBuffer.h"
#include <stdlib.h>
#include <algorithm>

//begin
#ifndef JASON_LEE_LANGUAGE
#define JASON_LEE_LANGUAGE
#define BEGIN_IF(con) if(con) {
#define BEGIN_ELSE_IF(con) } else if(con) {
#define BEGIN_ELSE } else {
#define END_IF }
#define BEGIN_WHILE_TRUE while(true) {
#define BEGIN_WHILE(con) while(con) {
#define END_WHILE }
#define BEGIN_DO_WHILE do {
#define END_DO_WHILE(con) } while(con);
#define END_DO_WHILE_TRUE } while(true);
#define BEGIN_FOR(a,con,c) for(a;con;c) {
#define BEGIN_FOR_TRUE(a,c) for(a;true;c) {
#define END_FOR }
#define DO_CONTINUE continue
#define DO_BREAK break
#define DO_ASSIGN(n,v) n = v
#define DO_ASSERT(c,m) CCAssert(c,m)
#define DO_RETURN_R(r) return r
#define DO_RETURN_FLASE return false
#define DO_RETURN_TRUE return true
#define DO_RETURN_NULL return NULL
#define DO_RETURN_Z return 0
#define DO_RETURN return
#define R_FALSE false
#define R_TRUE true
#define TO_SHORT(a) ((short)a)
#define TO_USHORT(a) ((unsigned short)a)
#define TO_INT(a) ((int)a)
#define TO_UINT(a) ((unsigned int)a)
#define IAA(a) a++
#define AAI(a) ++a
#define ISS(a) a--
#define SSI(a) --a
#define CA(a,b) (a + b)
#define CS(a,b) (a - b)
#define CM(a,b) (a * b)
#define CD(a,b) (a / b)
#define LX(a,b) (a < b)
#define LD(a,b) (a > b)
#define LE(a,b) (a == b)
#define LXE(a,b) (a <= b)
#define LDE(a,b) (a >= b)
#define LNE(a,b) (a != b)
#define LQ(a,b) (a && b)
#define LH(a,b) (a || b)
#define AA(a,b) (a += b)
#define AS(a,b) (a -= b)
#define AM(a,b) (a *= b)
#define AD(a,b) a = a / b
#define MAXA(a,b) a = (((a) < (b)) ? (b) : (a))
#define MINA(a,b) a = (((a) < (b)) ? (a) : (b))
#define QV(a) *(a)
#define QZ(a) &(a)
#endif 
//end

WWBuffer::WWBuffer() :
		_p_buffer(NULL), _u_write_pos(0), _u_read_pos(0), _u_content_size(0), _u_buffer_size(
		WWBuffer_ORGINAL_SIZE), _u_mark_pos(0) {
	DO_ASSIGN(_p_buffer, (char* ) malloc(_u_buffer_size));
}

WWBuffer::WWBuffer(int n_capacity) :
		_p_buffer(NULL), _u_write_pos(0), _u_read_pos(0), _u_content_size(0), _u_mark_pos(
				0) {
	DO_ASSIGN(_u_buffer_size, n_capacity);
	DO_ASSIGN(_p_buffer, (char* ) malloc(_u_buffer_size));
}

WWBuffer::WWBuffer(const char* p_data, int u_len) :
		_p_buffer(NULL), _u_read_pos(0), _u_mark_pos(0) {
	DO_ASSERT(p_data && LD(u_len, 0), "p_data && u_len > 0");

	DO_ASSIGN(_u_buffer_size, u_len);
	DO_ASSIGN(_u_write_pos, u_len);
	DO_ASSIGN(_u_content_size, u_len);
	DO_ASSIGN(_p_buffer, (char* ) malloc(u_len));

	memcpy(_p_buffer, p_data, u_len);
}

WWBuffer::~WWBuffer() {
	CC_SAFE_FREE(_p_buffer);
}

WWBuffer* WWBuffer::create() {
	DO_ASSIGN(WWBuffer* pRet, new WWBuffer());
	pRet->autorelease();
	DO_RETURN_R(pRet);
}

WWBuffer* WWBuffer::create(int n_capacity) {
	DO_ASSIGN(WWBuffer* pRet, new WWBuffer(n_capacity));
	pRet->autorelease();
	DO_RETURN_R(pRet);
}

WWBuffer* WWBuffer::create(const char* p_data, int u_len) {
	DO_ASSIGN(WWBuffer* pRet, new WWBuffer(p_data, u_len));
	pRet->autorelease();
	DO_RETURN_R(pRet);
}

WWBuffer* WWBuffer::copy() {
		BEGIN_IF(LD(_u_content_size, 0))
		DO_ASSIGN(WWBuffer* p_buf, create(_p_buffer, _u_content_size));
		DO_RETURN_R(p_buf);
		BEGIN_ELSE
		DO_RETURN_NULL;END_IF
}

unsigned int WWBuffer::getWriterIndex() const {
	DO_RETURN_R(_u_write_pos);
}

unsigned int WWBuffer::getContentSize() const {
	DO_RETURN_R(_u_content_size);
}

unsigned int WWBuffer::getReadableSize() const {
	DO_RETURN_R(CS(_u_content_size, _u_read_pos));
}

unsigned int WWBuffer::getReaderIndex() const {
	DO_RETURN_R(_u_read_pos);
}

unsigned int WWBuffer::getBufferSize() const {
	DO_RETURN_R(_u_buffer_size);
}

void WWBuffer::discardReadContent() {
	BEGIN_IF(LD(_u_read_pos, 0))
		moveLeft(_u_read_pos);
	END_IF
}

void WWBuffer::skipData(int u_len) {
	BEGIN_IF(LXE(CA(_u_read_pos, u_len), _u_content_size))
		AA(_u_read_pos, u_len);BEGIN_ELSE
		AA(_u_read_pos, (CS(_u_content_size, _u_read_pos)));END_IF
}

void WWBuffer::moveWriterIndexToFront() {
	DO_ASSIGN(_u_write_pos, 0);
}

void WWBuffer::moveWriterIndexToBack() {
	DO_ASSIGN(_u_write_pos, _u_content_size);
}

void WWBuffer::moveReaderIndexToFront() {
	DO_ASSIGN(_u_read_pos, 0);
}

void WWBuffer::moveReaderIndexToBack() {
	DO_ASSIGN(_u_read_pos, _u_content_size);
}

void WWBuffer::reset() {
	moveWriterIndexToFront();
	moveReaderIndexToFront();
	DO_ASSIGN(_u_mark_pos, 0);
	DO_ASSIGN(_u_content_size, 0);
}

void WWBuffer::markReaderIndex() {
	DO_ASSIGN(_u_mark_pos, _u_read_pos);
}

void WWBuffer::resetReaderIndex() {
	DO_ASSIGN(_u_read_pos, _u_mark_pos);
}

void WWBuffer::setReaderIndex(int u_pos) {
	DO_ASSERT(LXE(u_pos, _u_content_size), "LXE(u_pos, _u_content_size)");
	DO_ASSIGN(_u_read_pos, u_pos);
}

void WWBuffer::setWriterIndex(int u_pos) {
	DO_ASSERT(LXE(u_pos, _u_content_size), "LXE(u_pos, _u_content_size)");
	DO_ASSIGN(_u_write_pos, u_pos);
}

void WWBuffer::moveLeft(int u_len) {
	BEGIN_IF(LE(_u_content_size, 0))
		DO_RETURN;END_IF
	DO_ASSERT(LNE(u_len, 0), "LNE(u_len, 0)");
	BEGIN_IF(LDE(u_len, _u_content_size))
		clear();
	BEGIN_ELSE
		BEGIN_FOR(DO_ASSIGN( int i, u_len), LX(i, _u_content_size), AAI(i))
			DO_ASSIGN(QV(CA(_p_buffer, CS(i, u_len))), QV(CA(_p_buffer, i)));
			DO_ASSIGN(QV(CA(_p_buffer, i)), 0);END_FOR
		DO_ASSIGN(_u_write_pos,
				TO_UINT(MAX(0, CS(TO_INT(_u_write_pos), TO_INT(u_len)))));
		DO_ASSIGN(_u_read_pos,
				TO_UINT(MAX(0, CS(TO_INT(_u_read_pos), TO_INT(u_len)))));
		DO_ASSIGN(_u_mark_pos,
				TO_UINT(MAX(0, CS(TO_INT(_u_mark_pos), TO_INT(u_len)))));
		DO_ASSIGN(_u_content_size, CS(_u_content_size, u_len));END_IF
}

void WWBuffer::moveRight(int u_len) {
	BEGIN_IF(LE(_u_content_size, 0))
		DO_RETURN;END_IF
	_reallocBufferSizeInChanged(u_len);
	BEGIN_FOR(DO_ASSIGN(int i, CS(_u_content_size, 1)), LDE(i, 0), SSI(i))
		DO_ASSIGN(QV(CA(CA(_p_buffer, i), TO_INT(u_len))),
				QV(CA(_p_buffer, i)));
		DO_ASSIGN(QV(CA(_p_buffer, i)), 0);END_FOR
	AA(_u_write_pos, u_len);
	AA(_u_read_pos, u_len);
	AA(_u_mark_pos, u_len);
	AA(_u_content_size, u_len);
}

bool WWBuffer::isReadable() const {
	BEGIN_IF(LE(_u_content_size, 0))
		DO_RETURN_FLASE;END_IF
	DO_RETURN_R(LX(_u_read_pos, _u_content_size));
}

bool WWBuffer::isReadable(int u_len) const {
	BEGIN_IF(LH(LE(_u_content_size, 0), LE(u_len, 0)))
		DO_RETURN_FLASE;END_IF
	DO_RETURN_R(LXE(CA(_u_read_pos, u_len), _u_content_size));
}

int WWBuffer::length() const {
	DO_RETURN_R(getContentSize());
}

int WWBuffer::capacity() const {
	DO_RETURN_R(getBufferSize());
}

bool WWBuffer::empty() const {
	DO_RETURN_R(LE(length(), 0));
}

char* WWBuffer::data() const {
	DO_RETURN_R(_p_buffer);
}

void WWBuffer::clear() {
	DO_ASSIGN(_u_content_size, 0);
	DO_ASSIGN(_u_write_pos, 0);
	DO_ASSIGN(_u_read_pos, 0);
	DO_ASSIGN(_u_mark_pos, 0);
	memset(_p_buffer, 0, _u_buffer_size);
}

void WWBuffer::_reallocBufferSize() {
	DO_ASSIGN(_u_buffer_size, CM(_u_buffer_size, 2));
	DO_ASSIGN(_p_buffer, (char* ) realloc(_p_buffer, _u_buffer_size));
}

void WWBuffer::_reallocBufferSizeInChanged(int u_len) {
	BEGIN_WHILE_TRUE
		BEGIN_IF( LXE(CA(_u_write_pos, u_len), _u_buffer_size) )
			DO_BREAK;BEGIN_ELSE
			_reallocBufferSize();
			END_IF
			END_WHILE
}

void WWBuffer::writeData(const char* p_data, int u_len) {
		DO_ASSERT(LQ(p_data, LD(u_len,0)), "LQ(p_data, LD(u_len,0))");
				_reallocBufferSizeInChanged(u_len);
				memcpy(CA(_p_buffer, _u_write_pos), p_data, u_len);
				AA(_u_write_pos, u_len);
				MAXA(_u_content_size, _u_write_pos);
}

void WWBuffer::writeLengthAndData(const char* p_data, int u_len) {
	if(u_len>0xFFFF){
		writeUShort(0xFFFF);
			writeInt(u_len);
	}else{
		writeUShort(u_len);
	}
		writeData(p_data, u_len);
}

void WWBuffer::writeChar(char data) {
			_reallocBufferSizeInChanged(sizeof(char));
			DO_ASSIGN(QV(CA(_p_buffer, _u_write_pos)), data);
			AAI(_u_write_pos);
			MAXA(_u_content_size, _u_write_pos);
		}

		void WWBuffer::writeUChar(unsigned char data) {
			writeChar((char) data);
		}

		void WWBuffer::writeBoolean(bool isbool){
			writeChar(isbool?0x01:0x00);
		}

		void WWBuffer::writeShort(short data) {
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_data[sizeof(short)], {0});
			memcpy(p_data, (short*) (QZ(data)), sizeof(short));
			std::reverse(QZ(p_data[0]), QZ(p_data[sizeof(short)]));
			writeData(p_data, sizeof(short));
#else
			DO_ASSIGN(char* p_data, (char*)(QZ(data)));
			writeData(p_data, sizeof(short));
#endif
		}

		void WWBuffer::writeUShort(unsigned short data) {
			writeShort(TO_SHORT(data));
		}

		void WWBuffer::writeInt(int data) {
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_data[sizeof(int)], {0});
			memcpy(p_data, (char*) (QZ(data)), sizeof(int));
			std::reverse(QZ(p_data[0]), QZ(p_data[sizeof(int)]));
			writeData(p_data, sizeof(int));
#else
			DO_ASSIGN(char* p_data, (char*)(QZ(data)));
			writeData(p_data, sizeof(int));
#endif
		}

		void WWBuffer::writeUInt(unsigned int data) {
			writeInt(TO_INT(data));
		}

		void WWBuffer::writeLongLong(long long data) {
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_data[sizeof(long long)], {0});
			memcpy(p_data, (char*) (QZ(data)), sizeof(long long));
			std::reverse(QZ(p_data[0]), QZ(p_data[sizeof(long long)]));
			writeData(p_data, sizeof(long long));
#else
			DO_ASSIGN(char* p_data, (char*)(QZ(data)));
			writeData(p_data, sizeof(long long));
#endif
		}

		void WWBuffer::writeULongLong(unsigned long long data) {
			writeLongLong((long long) (data));
		}

		void WWBuffer::writeFloat(float data) {
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_data[sizeof(float)], {0});
			memcpy(p_data, (char*) (QZ(data)), sizeof(float));
			std::reverse(QZ(p_data[0]), QZ(p_data[sizeof(float)]));
			writeData(p_data, sizeof(float));
#else
			DO_ASSIGN(char* p_data, (char*)(QZ(data)));
			writeData(p_data, sizeof(float));
#endif
		}

		void WWBuffer::writeDouble(double data) {
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_data[sizeof(double)], {0});
			memcpy(p_data, (char*) (QZ(data)), sizeof(double));
			std::reverse(QZ(p_data[0]), QZ(p_data[sizeof(double)]));
			writeData(p_data, sizeof(double));
#else
			DO_ASSIGN(char* p_data, (char*)(QZ(data)));
			writeData(p_data, sizeof(double));
#endif
		}

		void WWBuffer::writeString(const char* p_data) {
			DO_ASSERT(LQ(p_data, LDE(strlen(p_data), 0)),
					"p_data, LDE(strlen(p_data), 0)");
			BEGIN_IF(LH(!p_data, LXE(strlen(p_data), 0)))
			writeInt(0);
			DO_RETURN;
			END_IF
			writeData(p_data, (int)strlen(p_data));
		}


		void WWBuffer::writeLengthAndString(const char* p_data) {

			BEGIN_IF(LH(!p_data, LXE(strlen(p_data), 0)))
			writeUInt(0);
			DO_RETURN;
			END_IF
			int len = strlen(p_data);
			if (len >= 0xFFFF) {
				writeUInt(0xFFFF);
				writeInt(len);
			} else {
				writeUInt(TO_UINT(len));
			}

			writeString(p_data);
		}

		char* WWBuffer::readWholeData() {
			BEGIN_IF(LNE(_u_content_size, 0))
			DO_ASSIGN(char* p_buffer, new char[_u_content_size]);
			memcpy(p_buffer, _p_buffer, _u_content_size);
			DO_ASSIGN(_u_read_pos, _u_content_size);
			DO_RETURN_R(p_buffer);
			BEGIN_ELSE
			DO_RETURN_NULL;
			END_IF
		}

		WWBuffer* WWBuffer::readData(int u_len) {
			BEGIN_IF(isReadable(u_len))
			DO_ASSIGN(char* p_data, (char* ) malloc(u_len));
			readData(p_data, u_len);
			DO_ASSIGN(WWBuffer* p_ret, new WWBuffer(p_data, u_len));
			p_ret->autorelease();
			CC_SAFE_FREE(p_data);
			DO_RETURN_R(p_ret);
			BEGIN_ELSE
			DO_ASSIGN(WWBuffer* p_ret, new WWBuffer());
			p_ret->autorelease();
			DO_RETURN_R(p_ret);
			END_IF
		}
		WWBuffer*  WWBuffer::readLengthAndData(){

			int len = TO_INT(readUInt());
			if(len <= 0)
				len = 0;
			BEGIN_IF(len>=0xFFFF)
			DO_RETURN(readData(readUInt()));
			BEGIN_ELSE
			DO_RETURN(readData(len));
			END_IF
		}
		void WWBuffer::readData(char* p_out_data, int u_len) {
			BEGIN_IF(isReadable(u_len))
			memcpy(p_out_data, CA(_p_buffer, _u_read_pos), u_len);
			AA(_u_read_pos, u_len);
			BEGIN_ELSE_IF(LQ(LDE(CS(_u_content_size, _u_read_pos),0), LNE(u_len, 0)))
			DO_ASSIGN(u_len, CS(_u_content_size, _u_read_pos));
			memcpy(p_out_data, CA(_p_buffer, _u_read_pos), u_len);
			AA(_u_read_pos, u_len);
			END_IF
		}

		char WWBuffer::readChar() {
			BEGIN_IF(isReadable(sizeof(char)))
			DO_ASSIGN(char ret, QV(CA(_p_buffer, _u_read_pos)));
			AA(_u_read_pos, sizeof(char));
			DO_RETURN_R(ret);BEGIN_ELSE
			DO_RETURN_Z;
			END_IF
		}

		unsigned char WWBuffer::readUChar() {
			DO_RETURN_R((unsigned char )readChar());
		}

		bool  WWBuffer::readBoolean(){
			DO_RETURN_R((bool)readChar());
		}

		short WWBuffer::readShort() {
			BEGIN_IF(isReadable(sizeof(short)))
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_temp[sizeof(short)], {0});
			memcpy(p_temp, CA(_p_buffer, _u_read_pos), sizeof(short));
			std::reverse(QZ(p_temp[0]), QZ(p_temp[sizeof(short)]));
			DO_ASSIGN(short ret, QV((short*)p_temp));
#else
			DO_ASSIGN(short ret, QV((short*)(CA(_p_buffer, _u_read_pos))));
#endif
			AA(_u_read_pos, sizeof(short));
			DO_RETURN_R(ret);BEGIN_ELSE
			DO_RETURN_Z;END_IF
		}

		unsigned short WWBuffer::readUShort() {
			DO_RETURN_R(TO_USHORT(readShort()));
		}

		int WWBuffer::readInt() {
			BEGIN_IF(isReadable(sizeof(int)))
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_temp[sizeof(int)], {0});
			memcpy(p_temp, CA(_p_buffer, _u_read_pos), sizeof(int));
			std::reverse(QZ(p_temp[0]), QZ(p_temp[sizeof(int)]));
			DO_ASSIGN(int ret, QV((int*)p_temp));
#else
			DO_ASSIGN(int ret, QV((int*)(CA(_p_buffer, _u_read_pos))));
#endif
			AA(_u_read_pos, sizeof(int));
			DO_RETURN_R(ret);BEGIN_ELSE
			DO_RETURN_Z;END_IF
		}

		int WWBuffer::readChar3ToInt() {

			BEGIN_IF(isReadable(3*sizeof(char)))
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_temp[sizeof(int)], {0});
			p_temp[0]=0;
			memcpy(p_temp+1, CA(_p_buffer, _u_read_pos), 3 * sizeof(char));
			std::reverse(QZ(p_temp[0]), QZ(p_temp[ sizeof(int)]));
			DO_ASSIGN(int ret, QV((int*)p_temp));

#else
			DO_ASSIGN(char p_temp[sizeof(int)], {0});
			p_temp[0]=0;
			memcpy(p_temp+1, CA(_p_buffer, _u_read_pos), 3 * sizeof(char));
			DO_ASSIGN(int ret, QV((int*)p_temp));
#endif
			AA(_u_read_pos, 3 * sizeof(char));
			DO_RETURN_R(ret);
			BEGIN_ELSE
			DO_RETURN_Z;END_IF

		}

		unsigned int WWBuffer::readUInt() {
			DO_RETURN_R(TO_UINT(readInt()));
		}

		long long WWBuffer::readLongLong() {
			BEGIN_IF(isReadable(sizeof(long long)))
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_temp[sizeof(long long)], {0});
			memcpy(p_temp, CA(_p_buffer, _u_read_pos), sizeof(long long));
			std::reverse(QZ(p_temp[0]), QZ(p_temp[sizeof(long long)]));
			DO_ASSIGN(long long ret, QV((long long*)p_temp));
#else
			DO_ASSIGN(long long ret, QV((long long*)(CA(_p_buffer, _u_read_pos))));
#endif
			AA(_u_read_pos, sizeof(long long));
			DO_RETURN_R(ret);BEGIN_ELSE
			DO_RETURN_Z;END_IF
		}

		unsigned long long WWBuffer::readULongLong() {
			DO_RETURN_R((unsigned long long )(readLongLong()));
		}

		float WWBuffer::readFloat() {
			BEGIN_IF(isReadable(sizeof(float)))
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_temp[sizeof(float)], {0});
			memcpy(p_temp, CA(_p_buffer, _u_read_pos), sizeof(float));
			std::reverse(QZ(p_temp[0]), QZ(p_temp[sizeof(float)]));
			DO_ASSIGN(float ret, QV((float*)p_temp));
#else
			DO_ASSIGN(float ret, QV((float*)(CA(_p_buffer, _u_read_pos))));
#endif
			AA(_u_read_pos, sizeof(float));
			DO_RETURN_R(ret);BEGIN_ELSE
			DO_RETURN_Z;END_IF
		}

		double WWBuffer::readDouble() {
			BEGIN_IF(isReadable((int)sizeof(double)))
#if MEMORYTYPE_REVERSE
			DO_ASSIGN(char p_temp[sizeof(double)], {0});
			memcpy(p_temp, CA(_p_buffer, _u_read_pos), sizeof(double));
			std::reverse(QZ(p_temp[0]), QZ(p_temp[sizeof(double)]));
			DO_ASSIGN(double ret, QV((double*)p_temp));
#else
			DO_ASSIGN(double ret, QV((double*)(CA(_p_buffer, _u_read_pos))));
#endif
			AA(_u_read_pos, sizeof(double));
			DO_RETURN_R(ret);BEGIN_ELSE
			DO_RETURN_Z;END_IF
		}

		std::string WWBuffer::readString(int u_len) {
            log("the u_len is%d",u_len);
			DO_ASSERT(LDE(u_len, 0), "LDE(u_len, 0)");
			std::string str;
			if(u_len<=0){
				return str;
			}
			DO_ASSIGN(char* p_data, (char*)malloc(CA(u_len, 1)));
			readData(p_data, u_len);
			DO_ASSIGN(p_data[u_len], 0);
			str.assign(p_data);
			CC_SAFE_FREE(p_data);
			return str;
		}

		std::string	WWBuffer::readLengthAndString()
		{
			int len = TO_INT(readUInt());
			BEGIN_IF(len>=0xFFFF)
				DO_RETURN(readString(readInt()));
			BEGIN_ELSE
				DO_RETURN(readString(len));
			END_IF

		}


