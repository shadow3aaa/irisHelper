#include "include/LOG.h"
#include "include/irisConfig.h"
#include <fstream>
#include <vector>
#include <string>
void print_struct(std::vector<irisConfig> &conf)
{
	for (const auto &tmp : conf) {
		LOG("包名: ", tmp.app);
		LOG("参数a: ", tmp.params_a);
		LOG("参数b: ", tmp.params_b);
		LOG("参数c: ", tmp.params_c);
		LOG("参数d: ", tmp.params_d);
		LOG("df: ", tmp.df);
		LOG("perfmgr是否开启: ", tmp.perfmgr_enable);
		LOG("目标fps: ", tmp.fixed_target_fps);
		LOG("\n");
	}
}
// 删除第一列
void GetSecondArg(std::string &buf)
{
	size_t pos = buf.find(' ');
	if (pos != std::string::npos) {
		buf.erase(0, pos + 1); // 删除空格和左边的所有内容
	}
}
static inline void reduceStr(std::string &buf)
{
	while (buf.find("\"") != std::string::npos) {
		buf.erase(buf.find("\""), 1);
	}
}
auto strCtrl(std::string &buf, const char *target_str, std::string &arg) -> bool
{
	if (buf.find(target_str) != std::string::npos) {
		GetSecondArg(buf);
		reduceStr(buf);
		arg = buf;
		return true;
	}
	return false;
}

auto readProfile(const char *profile, std::vector<irisConfig> &conf) -> bool
{
	std::ifstream file(profile);
	if (!file.is_open()) {
		return false;
	}
	std::string buf;
	while (true) {
		std::string app = "";
		std::string params_a = "";
		std::string params_b = "";
		std::string params_c = "";
		std::string params_d = "";
		std::string df = "";
		std::string perfmgr_enable = "0";
		std::string fixed_target_fps = "-1";
		bool flag = false;
		while (std::getline(file, buf)) {
			if (buf[0] == '#' || buf.empty()) {
				continue;
			}

			if (buf.find("app") != std::string::npos) {
				if ((app != "") && (flag = true)) {
					if (!(params_a == "" &&
					      params_b == "" &&
					      params_c == "" &&
					      params_d == "" && df == "" &&
					      perfmgr_enable == "0" &&
					      fixed_target_fps == "-1")) {
						conf.push_back(
							{ app.c_str(),
							  params_a.c_str(),
							  params_b.c_str(),
							  params_c.c_str(),
							  params_d.c_str(),
							  df.c_str(),
							  atoi(perfmgr_enable
								       .c_str()),
							  atoi(fixed_target_fps
								       .c_str()) });
					}

					// reset value
					params_a = "";
					params_b = "";
					params_c = "";
					params_d = "";
					df = "";
					perfmgr_enable = "0";
					fixed_target_fps = "-1";
				}
				GetSecondArg(buf);
				reduceStr(buf);
				app = buf;
				flag = true;
				continue;
			}

			if (strCtrl(buf, "params_a", params_a)) {
				continue;
			}
			if (strCtrl(buf, "params_b", params_b)) {
				continue;
			}
			if (strCtrl(buf, "params_c", params_c)) {
				continue;
			}
			if (strCtrl(buf, "params_d", params_d)) {
				continue;
			}
			if (strCtrl(buf, "df", df)) {
				continue;
			}
			if (strCtrl(buf, "perfmgr_enable", perfmgr_enable)) {
				if (perfmgr_enable != "0" &&
				    perfmgr_enable != "1") {
					perfmgr_enable = "0";
				}
				continue;
			}
			if (strCtrl(buf, "fixed_target_fps",
				    fixed_target_fps)) {
				continue;
			}
		}
		if (app == "") {
			break;
		}
		if (!(params_a == "" && params_b == "" && params_c == "" &&
		      params_d == "" && df == "" && perfmgr_enable == "0" &&
		      fixed_target_fps == "-1")) {
			conf.push_back({ app.c_str(), params_a.c_str(),
					 params_b.c_str(), params_c.c_str(),
					 params_d.c_str(), df.c_str(),
					 atoi(perfmgr_enable.c_str()),
					 atoi(fixed_target_fps.c_str()) });
		}
	}
	file.close();
	return true;
}
