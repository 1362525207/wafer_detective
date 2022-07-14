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
	string getjson_keyvalue(string keyname);//��þ����ֵ
	string getjson_keyvalue(string keyname1, string keyname2);//��þ����ֵ
	vector<int> getjson_array(string keyname);
	bool setjson_keyvalue(string keyname, string jsonnowvalue);//���þ����ֵ 
	bool setjson_arrayvalue(string keyname,int nowkey, string jsonnowvalue);//���þ����ֵ
	bool setjson_keyvalue(string keyname, int jsonnowvalue);//���þ����ֵ
	bool setjson_keyvalue(string keyname1, string keyname2, string jsonnowvalue);//���þ����ֵ 
private:
	char* jsonFileName;
	Value jsonInfo;
	mutex nowmutext;
};

