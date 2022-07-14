#pragma once
#include <afx.h>
#include <json/json.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <mutex>
using namespace std;
using namespace Json;
class wafer_json: public CObject
{
public:
	wafer_json(char* _jsonFileName) :jsonFileName(_jsonFileName) {}
	bool readJsonFile(const char* jsonFileName, Value* jsonInfo);
	bool writeJsonFile(const char* jsonFileName, const Value writeJson);
	string getjson_keyvalue(string keyname);//获得具体的值
	string getjson_keyvalue(string keyname1, string keyname2);//获得具体的值
	vector<int> getjson_array(string keyname);
	bool setjson_keyvalue(string keyname, string jsonnowvalue);//设置具体的值 
	bool setjson_arrayvalue(string keyname,int nowkey, string jsonnowvalue);//设置具体的值
	bool setjson_keyvalue(string keyname, int jsonnowvalue);//设置具体的值
	bool setjson_keyvalue(string keyname1, string keyname2, string jsonnowvalue);//设置具体的值 
private:
	char* jsonFileName;
	Value jsonInfo;
	mutex nowmutext;
};

