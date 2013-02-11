#include <iostream>
#include <fstream> //ifstream
#include <sstream> //stringstream
#include <iomanip> //setw
#include <deque>
#include <string>

struct EVERYHOUR
{
    //UTC+0 日日时时
    unsigned int HourTime;
    double  HourSumElect;
    double  HourSumPrice;
    double  HourConsumeElect;
    double  HourConsumePrice;
};

struct EVERYHOUR Everyhour = {0};

//////////////////////////////////////////////////////
#define checkErrors(err)	__checkErrors (err, __FILE__, __LINE__)

class ErrorReport
{
public:
    enum ERROR_DEFINE
    {
        NO_ERROR = 0,
        UNWRITE_FILE,
        UNREAD_FILE,
        NO_INPUT,
    };

    //  inline void __checkErrors(const int err, const char *file, const int line )
    inline void __checkErrors(const ERROR_DEFINE err, const char *file, const int line )
    {
        if(NO_ERROR != err)
        {
            std::cerr << "Error: " << std::endl
                      << "--file:" << file << std::endl
                      << "--line :" << line << std::endl
                      << "--msg:" << GetErrorString(err) << std::endl;
            getchar();
            exit(-1);
        }
    }

    ErrorReport() {}

private:
    //不必提前声明
    // inline char *GetErrorString(const int err)
    inline char *GetErrorString(const ERROR_DEFINE err)
    {
        switch(err)
        {
        case UNWRITE_FILE:
            return "unable write file";
        case UNREAD_FILE:
            return "unable read file";
        case NO_INPUT:
            return "Usage: 201102.b <path don't chinese>";
        default :
            return "unkown";
        }
    }
};

/////////////////////////////////////////////////////////

int main( int nArgc, char **papszArgv )
{
    ErrorReport er;

    if(nArgc <= 1)
        er.checkErrors(er.NO_INPUT);

    std::deque<char *> pszFilename;
    std::deque<std::string> outfilename_str;

    for( int iArg = 1; iArg < nArgc; iArg++ )
    {
        if(NULL == papszArgv[iArg] )
            er.checkErrors(er.NO_INPUT);

        pszFilename.push_back(papszArgv[iArg]);

        std::stringstream outfilename;
        outfilename << pszFilename.back() << ".txt";

        outfilename_str.push_back(outfilename.str());
    }

    std::deque<char *>::iterator cb = pszFilename.begin(),
                                 ce = pszFilename.end();
    std::deque<std::string>::iterator ob = outfilename_str.begin(),
                                      oe = outfilename_str.end();

    for( unsigned iArg = 0; iArg < pszFilename.size(); iArg++, cb++, ob++)
    {
        std::ofstream outfs(ob->c_str(), std::ofstream::out);

        if(outfs)
        {
            outfs << "比如2302\n23号02:00-02:59区间的耗电量和电费\n02:59时刻的电表读数和总价格" << std::endl;
            outfs << "日时\t小时末电表读数\t小时末总价格\t该小时耗电量\t该小时的电费" << std::endl;

            std::ifstream infs(*cb, std::ifstream::in | std::ifstream::binary);

            if(infs)
            {
                //while(!infs.eof())
                while(infs.good())
                {
                    infs.read(reinterpret_cast<char *>(&Everyhour), sizeof(struct EVERYHOUR));

                    outfs << std::setw(4) << std::setfill('0') << Everyhour.HourTime << "\t";
                    outfs << Everyhour.HourSumElect << "\t" <<
                          Everyhour.HourSumPrice << "\t" <<
                          Everyhour.HourConsumeElect << "\t" <<
                          Everyhour.HourConsumePrice << std::endl;
                }

                infs.close();
                infs.clear();
                outfs.close();
                outfs.clear();
            }
            else
            {
                outfs.close();
                outfs.clear();
                er.checkErrors(er.UNREAD_FILE);
            }
        }
        else
            er.checkErrors(er.UNWRITE_FILE);
    }
}