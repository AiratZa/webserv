//
// Created by airat on 14.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../../ServerContext.hpp"
#include "../fixtures.hpp"



struct ConfigParams
{
    std::string conf_text;

    std::map<int, std::map<std::string, std::string> > err_codes_info;

    ConfigParams(const std::string& conf_text) : conf_text(conf_text) { };

    ConfigParams(const std::string& conf_text, int err_code_1, int err_code_2, int err_code_3,
                 const std::string& err_to_change,
                 const std::string& redirect_uri) : conf_text(conf_text){

        std::list<int> err_codes;
        if (err_code_1 > 0)
            err_codes.push_back(err_code_1);
        if (err_code_2 > 0)
            err_codes.push_back(err_code_2);
        if (err_code_3 > 0)
            err_codes.push_back(err_code_3);

        std::list<int>::const_iterator it = err_codes.begin();

        std::map<std::string, std::string> params;
        params[ERROR_PAGE_CHANGE_ERROR_CODE] = err_to_change;
        params[ERROR_PAGE_REDIRECT_URI] = redirect_uri;

        while (it != err_codes.end()) {
            err_codes_info[*it] = params;
            ++it;
        }
    }

};

std::ostream & operator<<(std::ostream & output, ConfigParams const & rhs) {
    output << rhs.conf_text << std::endl;
    return output;
}

struct ErrorPageDirectiveParsingTests : testing::Test, testing::WithParamInterface<ConfigParams>
{
    ErrorPageDirectiveParsingTests()
    {
        conf_text = GetParam().conf_text;
    }

    std::string conf_text;
};

////NEGATIVE SCENARIOS

struct ErrorPageDirectiveParsingNegativeTests : ErrorPageDirectiveParsingTests { };

TEST_P(ErrorPageDirectiveParsingNegativeTests, NegativeScenarios)
{
    const ConfigParams& param =  GetParam();
    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/directives/tmp_config.conf";
    createTestConfigFromString(conf_file_name, param.conf_text);

    EXPECT_THROW({ Config conf = Config(conf_file_name); }, Config::BadConfigException);
}

INSTANTIATE_TEST_CASE_P(TestFromWhatErrorSingleValueChange, ErrorPageDirectiveParsingNegativeTests, testing::Values(
        ConfigParams("server{error_page 299 www.example.com;}"),
        ConfigParams("server{error_page -1 www.example.com;}"),
        ConfigParams("server{error_page a356 www.example.com;}")
    )
);


INSTANTIATE_TEST_CASE_P(TestFromWhatErrorMultipleValueChange, ErrorPageDirectiveParsingNegativeTests, testing::Values(
        ConfigParams("server{error_page 301 299 www.example.com;}"),
        ConfigParams("server{error_page 500 -1 www.example.com;}"),
        ConfigParams("server{error_page 404 a356 www.example.com;}")
)
);


INSTANTIATE_TEST_CASE_P(TestWhatErrorChangeToChange, ErrorPageDirectiveParsingNegativeTests, testing::Values(
        ConfigParams("server{error_page 404 = 200 www.example.com;}"),
        ConfigParams("server{error_page 500 = abc www.example.com;}"),
        ConfigParams("server{error_page 500 =404 =500 www.example.com;}"),
        ConfigParams("server{error_page 500 =404 =-500 www.example.com;}"),
        ConfigParams("server{error_page 500 ! www.example.com;}"),
        ConfigParams("server{error_page 500 =-1 www.example.com;}"),
        ConfigParams("server{error_page 404 =9999999999999999990 www.example.com;}")
)
);

INSTANTIATE_TEST_CASE_P(TestErrorPageCommon, ErrorPageDirectiveParsingNegativeTests, testing::Values(
        ConfigParams("server{error_page 500;}")
)
);

//////POSITIVE SCENARIOS HOST

struct ErrorPageDirectiveParsingPositiveTests : ErrorPageDirectiveParsingTests { };


TEST_P(ErrorPageDirectiveParsingPositiveTests, PositiveScenarios)
{
    const ConfigParams& param =  GetParam();
    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/directives/tmp_config.conf";
    createTestConfigFromString(conf_file_name, param.conf_text);

    Config conf = Config(conf_file_name);
    const std::list<ServerContext*>& servs = conf.getServersList();

    const std::map<int, std::map<std::string, std::string> >& codes = (*(servs.begin()))->getErrorPagesDirectiveInfo();

    std::cout << codes << std::endl;
    ASSERT_TRUE((param.err_codes_info.size() == codes.size()) &&
                            std::equal(codes.begin(), codes.end(), param.err_codes_info.begin())); // maps equality check
}




INSTANTIATE_TEST_CASE_P(TestWhatErrorChangeToChange, ErrorPageDirectiveParsingPositiveTests, testing::Values(
        ConfigParams("server{error_page 500 = www.example.com;}", 500, -1, -1, "=", "www.example.com"),
        ConfigParams("server{error_page 500 404 = www.example.com;}", 500, 404, -1, "=", "www.example.com"),
        ConfigParams("server{error_page 500 404 =0 www.example.com;}", 500, 404, -1, "=0", "www.example.com"),
        ConfigParams("server{error_page 404 =999999999999999999 www.example.com;}", 404, -1, -1, "=999999999999999999", "www.example.com"),
        ConfigParams("server{error_page 404 500 =999999999999999999 www.example.com;}", 404, 500, -1, "=999999999999999999", "www.example.com")
)
);

INSTANTIATE_TEST_CASE_P(TestErrorPageRedirectUri, ErrorPageDirectiveParsingPositiveTests, testing::Values(
        ConfigParams("server{error_page 500 = www.example.com;}", 500, -1, -1, "=", "www.example.com"),
        ConfigParams("server{error_page 500 = /www.example.com;}", 500, -1, -1, "=", "/www.example.com"),
        ConfigParams("server{error_page 500 =/www.example.com;}", 500, -1, -1, "", "=/www.example.com"),
        ConfigParams("server{error_page 500 =www.example.com;}", 500, -1, -1, "", "=www.example.com"),
        ConfigParams("server{error_page 500 = /50x.php;}", 500, -1, -1, "=", "/50x.php")
)
);