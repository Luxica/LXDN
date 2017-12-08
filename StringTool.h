#ifndef STRINGTOOL_H_
#define STRINGTOOL_H_

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni.h"
#endif
//#include "../LogSystem/WWLogMaros.h"

class StringTool {
public:
	StringTool();
	virtual ~StringTool();
	static std::vector<std::string> split(std::string src, std::string separate_character);
	static std::vector<std::string> splitByArr(std::string src, std::string separate_character);
	static std::string removeRichString(std::string src);
	static std::string replaceAll(std::string src, std::string org_str, std::string rep_str);
	// 查找子串sub在str中出现的次数--用C++方式--子串可重叠
	static int getTimesWithSame(const std::string str, const std::string sub);
	// 查找子串sub在str中出现的次数--用C方式--子串可重叠
	static int getTimesWithSame(const char* str, const char* sub);
	// 查找子串sub在str中出现的次数--用C++方式--子串不可重叠
	static int getTimesWithNoSame(const std::string str, const std::string sub);
	// 查找子串sub在str中出现的次数--用C方式--子串不可重叠
	static int getTimesWithNoSame(const char* str, const char* sub);
    //截取字符串定长,其余替换为"..."
    static std::string getStringLenth(std::string str,int length);
    static void urlEncode(std::string &sourceUrl, std::string &outUrl);
    
    //float转换为string
    static std::string floatToString(float floatNum);
    
    //int转换为string
    static std::string intToString(int intNum);
    
    //longlong转string
    static std::string longlongToString(long long longNum);
    
    //string转换为int
    static int stringToint(const std::string& string);
	static long long stringToLongLong(const std::string &string);
    
    //去掉字符串中所有的includeStr 最常用的是去掉回车
    static std::string removeIncludeStr(std::string pointStr,std::string includeStr);
    
    static bool startWith(const std::string& str, const std::string& strStart);
    static bool endWith(const std::string& str, const std::string& strEnd);

	/*
	 * 使用c++11 变长参数模板(Variadic templates)特性，用以将多种不同类型的变量转换并链接成字符串
	 */
	template <typename ...ARGS>
	static std::string toString(const ARGS &...args)
	{
		return toStringHelper(args...);
	}

private:
	template <typename T, typename ...ARGS>
	static std::string toStringHelper(const T &t, const ARGS &...args)
	{
		std::stringstream ss;
		ss << t << toStringHelper(args...);
		return ss.str();
	}

	template <typename T>
	static std::string toStringHelper(const T &t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	static std::string toStringHelper()
	{
		return "";
	}
};

#endif /* StringTool_H_ */
