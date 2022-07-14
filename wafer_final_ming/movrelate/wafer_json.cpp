#include "pch.h"
#include "wafer_json.h"
bool wafer_json::readJsonFile(const char* jsonFileName, Json::Value* jsonInfo) {
    ifstream jsonFile(jsonFileName, ios::binary);
    if (!jsonFile.is_open()) return false;
    else
    {
        Json::Reader reader;
        if (reader.parse(jsonFile, *jsonInfo))
        {
            jsonFile.close();
            return true;
        }
        else
        {
            printf("parse json ERROR\n");
            jsonFile.close();
            return false;
        }

    }
    return true;
}
bool wafer_json::writeJsonFile(const char* jsonFileName, const Json::Value writeJson) {
    ofstream jsonFile(jsonFileName, ios::out);
    if (!jsonFile.is_open()) { return false; }
    else
    {
        Json::StyledWriter sw;
        jsonFile << sw.write(writeJson);
        jsonFile.close();
        return true;
    }
    return true;
}
string wafer_json::getjson_keyvalue(string keyname) {
    nowmutext.lock();
    readJsonFile(jsonFileName, &jsonInfo);
    ifstream injsonFile(jsonFileName, ios::binary);
    Reader reader;
    if (!reader.parse(injsonFile, this->jsonInfo)) return "0";//刷新信息
    string ans = this->jsonInfo[keyname].asString();
    injsonFile.close();
    nowmutext.unlock();
    return ans;
}
string wafer_json::getjson_keyvalue(string keyname1, string keyname2) {
    ifstream injsonFile(jsonFileName, ios::binary);
    Reader reader;
    if (!reader.parse(injsonFile, this->jsonInfo)) return "0";//刷新信息
    string ans = this->jsonInfo[keyname1][keyname2].asString();
    injsonFile.close();
    return this->jsonInfo[keyname1][keyname2].asString();
}
bool wafer_json::setjson_keyvalue(string keyname1, string keyname2, string jsonnowvalue) {
    readJsonFile(jsonFileName, &jsonInfo);
    ofstream outjsonFile(jsonFileName, ios::out);
    StyledWriter sw;
    jsonInfo[keyname1][keyname2] = jsonnowvalue;
    outjsonFile << sw.write(jsonInfo);
    return true;
}
bool wafer_json::setjson_keyvalue(string keyname, string jsonnowvalue) {
    readJsonFile(jsonFileName, &jsonInfo);
    ofstream outjsonFile(jsonFileName, ios::out);
    StyledWriter sw;
    jsonInfo[keyname] = jsonnowvalue;
    outjsonFile << sw.write(jsonInfo);
    return true;
}
bool wafer_json::setjson_keyvalue(string keyname, int jsonnowvalue) {
    readJsonFile(jsonFileName, &jsonInfo);
    ofstream outjsonFile(jsonFileName, ios::out);
    StyledWriter sw;
    jsonInfo[keyname] = jsonnowvalue;
    outjsonFile << sw.write(jsonInfo);
    return true;
}
vector<int> wafer_json::getjson_array(string keyname) {
    ifstream injsonFile(jsonFileName, ios::binary);
    Reader reader;
    if (!reader.parse(injsonFile, this->jsonInfo)) return {};//刷新信息
    Value now = this->jsonInfo[keyname];
    vector<int> ans;
    for (unsigned int i = 0; i < now.size(); i++) {
        ans.push_back(now[i].asInt());
    }
    injsonFile.close();
    return ans;
}

bool wafer_json::setjson_arrayvalue(string keyname, int nowkey, string jsonnowvalue) {
    readJsonFile(jsonFileName, &jsonInfo);
    ofstream outjsonFile(jsonFileName, ios::out);
    StyledWriter sw;
    jsonInfo[keyname][nowkey] = stoi(jsonnowvalue);
    outjsonFile << sw.write(jsonInfo);
    return true;
}