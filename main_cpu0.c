#include "global.h"
#include "thread.h"
#include "main.h"

static thread_info_t m_thread_info;

void main_cpu0_unit_test(void)
{
    pthread_mutex_lock(&foo.mutex);
    printf("[Thread %d]: %d\n", m_thread_info.thread_id, foo.val);
    pthread_mutex_unlock(&foo.mutex);
}

void *main_cpu0(void *para)
{
    int ret;

    m_thread_info.thread = ((thread_info_t *)para)->thread;
    m_thread_info.thread_id = ((thread_info_t *)para)->thread_id;
    m_thread_info.sleep_nsec = ((thread_info_t *)para)->sleep_nsec;

    struct timespec request;
    request.tv_sec = 0;
    request.tv_nsec = m_thread_info.sleep_nsec;

    while (1) {
        main_cpu0_unit_test();

        if (m_thread_info.sleep_nsec) {
            ret = nanosleep(&request, NULL);
            if (ret == -1)
                printf("nanosleep error, errno=%d [%s]\n",
                    request.tv_nsec, errno, strerror(errno));
        }
    }
    pthread_exit(NULL);
}