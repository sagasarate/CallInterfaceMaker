#pragma once

inline bool GetJsonValue(rapidjson::Value& JsonValue, bool& OutValue)
{
	if (JsonValue.IsBool())
	{
		OutValue = JsonValue.GetBool();
		return true;
	}
	return false;
}

inline bool GetJsonValue(rapidjson::Value& JsonValue, BYTE& OutValue)
{
	if (JsonValue.IsUint())
	{
		OutValue = JsonValue.GetUint();
		return true;
	}
	return false;
}

inline bool GetJsonValue(rapidjson::Value& JsonValue, int& OutValue)
{
	if (JsonValue.IsInt())
	{
		OutValue = JsonValue.GetInt();
		return true;
	}
	return false;
}

inline bool GetJsonValue(rapidjson::Value& JsonValue, UINT& OutValue)
{
	if (JsonValue.IsUint())
	{
		OutValue = JsonValue.GetUint();
		return true;
	}
	return false;
}

inline bool GetJsonValue(rapidjson::Value& JsonValue, CString& OutValue)
{
	if (JsonValue.IsString())
	{
		OutValue = JsonValue.GetString();
		return true;
	}
	return false;
}

inline bool IsXMLFile(LPCTSTR FilePath)
{
	size_t Len = _tcslen(FilePath);
	if (Len >= 4)
	{
		return _tcsicmp(FilePath + Len - 4, _T(".xml")) == 0;
	}
	return false;
}