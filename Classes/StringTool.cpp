#include "StringTool.h"
#include <assert.h>
#include <string>

StringTool::StringTool() {
	// TODO Auto-generated constructor stub

}

StringTool::~StringTool() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string> StringTool::split(std::string src, std::string separate_character)
{
	std::vector<std::string> strs;
	int separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符
	int last_position = 0, index = -1;
	while (-1 != (index = src.find(separate_character, last_position)))
	{
//		WW_LOG_INFO("Charge::======================:%s", src.substr(last_position, index - last_position).c_str());
		strs.push_back(src.substr(last_position, index - last_position));
		last_position = index + separate_characterLen;
	}
	std::string lastString = src.substr(last_position);
	if (!lastString.empty())
	{
//		WW_LOG_INFO("Charge::======================:%s", lastString.c_str());
		strs.push_back(lastString);
	}
	return strs;
}
std::vector<std::string> StringTool::splitByArr(std::string src, std::string separate_character)
{
	std::vector<std::string> strs;
	auto separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符
	auto last_position = 0, index = -1;
//	WW_LOG_INFO("splitByArr::src======================:%s", src.c_str());
	while (-1 != (index = src.find(separate_character, last_position)))
	{
//		WW_LOG_INFO("splitByArr:: split======================:%s", src.substr(last_position, index - last_position).c_str());
		strs.push_back(src.substr(last_position, index - last_position).c_str());
		last_position = index + separate_characterLen;
	}
	std::string lastString = src.substr(last_position);
	if (!lastString.empty())
	{
//		WW_LOG_INFO("splitByArr:: last ======================:%s", lastString.c_str());
		strs.push_back(lastString.c_str());
//		WW_LOG_INFO("splitByArr:: last  add======================:%s", lastString.c_str());
	}
//	WW_LOG_INFO("splitByArr::strs count======================:%d", strs->count());
	return strs;
}

std::string StringTool::removeRichString(std::string src)
{
	std::vector<std::string> strs;
//	int separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符
	std::string separate_character_left = "[meta";
	std::string separate_character_right = "/]";
	int last_position = 0, index = -1;
	while (-1 != (index = src.find(separate_character_left, last_position)))
	{
//		WW_LOG_INFO("Charge::======================:%s", src.substr(last_position, index - last_position).c_str());
		strs.push_back(src.substr(last_position, index - last_position));
		int separate_characterLen = src.find(separate_character_right, last_position)+separate_character_right.length() - index;
		last_position = index + separate_characterLen;
	}
	std::string lastString = src.substr(last_position);
	if (!lastString.empty())
	{
//		WW_LOG_INFO("Charge::======================:%s", lastString.c_str());
		strs.push_back(lastString);
	}

	std::string str = "";
	for(int i=0; i<strs.size(); i++){
		str.append(strs[i]);
	}

	std::string result = str;
    
    //去掉@
    while (result.find("@") != std::string::npos)
    {
        result = result.replace(result.find("@"), 1, "");
    }
    
	return result;
}

void StringTool::urlEncode(std::string &sourceUrl, std::string &outUrl)
{
    char const *in_str = sourceUrl.c_str();
    int in_str_len = strlen(in_str);
    int out_str_len = 0;
    register unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    unsigned char hexchars[] = "0123456789ABCDEF";
    
    from = (unsigned char *)in_str;
    end = (unsigned char *)in_str + in_str_len;
    start = to = (unsigned char *)malloc(3 * in_str_len + 1);
    
    while (from < end)
    {
        c = *from++;
        
        if (c == ' ')
        {
            *to++ = '+';
        }
        else if ((c < '0' && c != '-' && c != '.') ||
                 (c < 'A' && c > '9') ||
                 (c > 'Z' && c < 'a' && c != '_') ||
                 (c > 'z'))
        {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
        }
        else
        {
            *to++ = c;
        }
    }
    
    *to = 0;
    out_str_len = to - start;
    outUrl = (char *)start;
    free(start);
}

std::string StringTool::replaceAll(std::string src, std::string org_str, std::string rep_str)
{
	std::vector<std::string> delimVec = split(src, org_str);
    if (delimVec.size() <= 0)
    {
        return src;
    }
    std::string target("");
    for (int i=0; i<delimVec.size(); i++)
    {
    	if(i == delimVec.size() - 1)
    	{
    		target = target + delimVec[i];
    	}
    	else
    	{
    		target = target + delimVec[i] + rep_str;
    	}
//       WW_LOG_INFO("Charge::======================:%s", target.c_str());
    }
    return target;
}


// 查找子串sub在str中出现的次数--用C++方式--子串可重叠
int StringTool::getTimesWithSame(const std::string str, const std::string sub)
{
	int num = 0;
	for (size_t i=0; (i=str.find(sub,i)) != std::string::npos; num++, i++);
	return num;
}

//函数2: 查找子串sub在str中出现的次数--用C方式--子串可重叠
int StringTool::getTimesWithSame(const char* str, const char* sub)
{
	int num = 0;
	for (const char* pstr=str; *pstr && (pstr=strstr(pstr,sub)); pstr++,num++);
	return num;
}
//函数3: 查找子串sub在str中出现的次数--用C++方式--子串不重叠
int StringTool::getTimesWithNoSame(const std::string str, const std::string sub)
{
	int num = 0;
	size_t len = sub.length();
	if (len == 0)len=1;//应付空子串调用
	for (size_t i=0; (i=str.find(sub,i)) != std::string::npos; num++, i+=len);
	return num;
}

//函数4: 查找子串sub在str中出现的次数--用C方式--子串不重叠
int StringTool::getTimesWithNoSame(const char* str, const char* sub)
{
	int num = 0;
	size_t len = strlen(sub);
	if (len == 0)len=1;//应付空子串调用
	for (const char* pstr=str; *pstr && (pstr=strstr(pstr,sub)); pstr+=len,num++);
	return num;
}

std::string StringTool::floatToString(float floatNum)
{
    std::ostringstream oss;
    oss<<floatNum;
    std::string str(oss.str());
	return str;
}

std::string StringTool::intToString(int intNum)
{
    std::ostringstream oss;
    oss<<intNum;
    std::string str(oss.str());
	return str;
}

std::string StringTool::longlongToString(long long longNum)
{
    std::ostringstream oss;
    oss<<longNum;
    std::string str(oss.str());
    return str;
}

std::string StringTool::getStringLenth(std::string str,int length)
{
    auto holelength = str.length();
	if (holelength < length)
		return str;
    auto afterReplace = str.replace(length, holelength-length, "...");
    return afterReplace;
}

int StringTool::stringToint(const std::string& string)
{
    int i = static_cast<int>(strtol(string.c_str(),NULL,10));
    return i;
}

long long StringTool::stringToLongLong(const std::string &string)
{
	long long ret = strtoll(string.c_str(), nullptr, 10);
	return ret;
}

std::string StringTool::removeIncludeStr(std::string pointStr,std::string includeStr)
{
    CC_ASSERT(!pointStr.empty());
    if (pointStr.empty())
    {
        return pointStr;
    }
    auto pos = pointStr.find(includeStr);
    if (pos>pointStr.size())
    {
        return pointStr;
    }
    pointStr.replace(pos, includeStr.size(), "");
    return  removeIncludeStr(pointStr, includeStr);
}

bool StringTool::startWith(const std::string& str, const std::string& strStart)
{
    if(str.empty() || strStart.empty())
    {
        return false;
    }

    return str.compare(0, strStart.size(), strStart) == 0 ? true : false;
}

bool StringTool::endWith(const std::string& str, const std::string& strEnd)
{
    if(str.empty() || strEnd.empty())
    {
        return false;
    }
    return str.compare(str.size() - strEnd.size(), strEnd.size(), strEnd) == 0 ? true : false;
}
