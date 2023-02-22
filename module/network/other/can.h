#if 0
#include <string>
#include "util/thread.h"
#include "public/define.h"
#include "basetype/string.h"
//¿©’π÷°
#define EFF 0x8000000U
//‘∂≥Ã÷°
#define RTR 0x4000000U
//¥ÌŒÛ÷°
#define ERR 0x2000000U
namespace newobj
{
    class newobj;
    namespace network
    {
        typedef void(newobj::* CALLBACK_CAN_RECV)(uint32 id, const char* data, uint32 size);

        /********************************************
         * Class:CANÕ®–≈
         *******************************************/
        class MNETWORK_API can :thread
        {
        public:
            can();
            ~can();
            bool init(const nstring& can_name, uint32 canID);
            bool send(const char* buff, ushort size, uint32 can_flag = 0);

            void on_recv(newobj* class_ptr, CALLBACK_CAN_RECV recv_cb);
        private:
            bool run();
        private:
            int m_fd = 0;
            uint32 m_id = 0;
            CALLBACK_CAN_RECV m_recv_cb = NULL;
            newobj* m_recv_cb_class_ptr = NULL;
        };
    }
   


}
#endif