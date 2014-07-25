#include "defs.h"
#include "monitor.h"
#include <pthread.h>
#include <sys/time.h>
#include <vector>
#include <fstream>
#include <unistd.h>

/* ------------------------------------------ */
/*              monitor                       */
/* ------------------------------------------ */
timeval starttime;
extern double cpuInfo[TOTAL_CORE];

bool cpuStart = true;
struct cpuRecorder {
    std::vector<long> curr;
    std::vector<std::vector<double> > cpus;
} cRecorder;

long get_current_time()
{
    struct timeval endtime;
    gettimeofday(&endtime, 0);
    long timeuse  = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    return timeuse;
}
void * thr_fn(void *arg)
{
    int t = 0;
    while(cpuStart)
    {
        usleep(50000);
        getCpuInfo();
        long timeuse = get_current_time();
        std::vector<double> tmp;
        for(int i = 0; i < TOTAL_CORE; ++i)
            tmp.push_back(cpuInfo[i]);
        cRecorder.curr.push_back(timeuse);
        cRecorder.cpus.push_back(tmp);
    }
}
pthread_t ntid;

int main(int argc, char *argv[])
{
    std::string s = "/home/sherry/FF_VS/FF_0521/SSCA2v2.2/dumpbc.data";
    graph g = read_data(s);
    std::cout<<"G.n : "<<g.n<<std::endl;
    std::cout<<"G.m : "<<g.m<<std::endl;
    std::cout<<"k4approx: "<<g.k4approx<<std::endl;


    
 //   auto bc = seq_get_bc(g);
    auto bc = seq_get_bc(g);
    
    /* ------------------------------------------ */
    /*              monitor                       */
    /* ------------------------------------------ */
    gettimeofday(&starttime,0);
    //long curr = get_current_time();
    int err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
        printf("failed to get cpuinfo %s\n");
    
    bc = tbb_get_bc(g);
//    auto bc = tbb_get_bc(g);
    
    /* ------------------------------------------ */
    /*              monitor                       */
    /* ------------------------------------------ */
    cpuStart = false;
    pthread_join(ntid, NULL);
    printf("join\n");
    //#ifdef FILE_LOG
    //{
    char filename[40];
    sprintf(filename, "cpu.log");
    std::ofstream os(filename);
    for(int i = 0; i < cRecorder.curr.size(); ++i)
    {
        //os << cRecorder.curr[i] << std::endl;
        for(int j = 0; j < cRecorder.cpus[i].size(); ++j)
        {
            os << j << " ";
            os << (float)(cRecorder.curr[i]/1000)/1000 << " ";
            os << cRecorder.cpus[i][j];
            os << std::endl;
        }
    }
    //}
    //#endif

    //auto bc = boost::shared_ptr<DOUBLE_T[]>(new DOUBLE_T[g.n]);
    //betweennessCentrality(&g, bc.get());
    double sum1 = 0;
    double sum_origin = 0;
    for(int i = 0; i < g.n; i++)
    {
        sum1 += bc[i];
        sum_origin += g.BC[i];
    }

    std::cout<<"BC FF: "<<sum1<<std::endl;
    std::cout<<"BC Origin: "<<sum_origin<<std::endl;
    return 0;
}
