#pragma once

#define from_num(NUMBER) nstring::from((int64)NUMBER).c_str()


#define for_l(LOOP_VAR,LOOP_NUM) for(long long LOOP_VAR = 0;LOOP_VAR < LOOP_NUM;LOOP_VAR ++)
#define for_iter(LOOP_VAR,LOOP_NUM) for(auto LOOP_VAR = LOOP_NUM.begin();LOOP_VAR != LOOP_NUM.end();LOOP_VAR++)
#define for_riter(LOOP_VAR,LOOP_NUM) for(auto LOOP_VAR = LOOP_NUM.rbegin();LOOP_VAR != LOOP_NUM.rend();LOOP_VAR++)
#define map_find(MAP_VAR,FIND_VAR) MAP_VAR.find(FIND_VAR) != MAP_VAR.end()

#define BREAK_OFF abort()


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef EXEC
#define trw  newobj::exception e_newobj;e_newobj.set_logptr(global::core->m_log_ptr);e_newobj.set_codeaddr(__LINE__,__FILE__,__FUNCTION__);throw e_newobj
#define trw_str(exp_str)  newobj::exception e_newobj((exp_str));e_newobj.set_logptr(global::core->m_log_ptr);e_newobj.set_codeaddr(__LINE__,__FILE__,__FUNCTION__);throw e_newobj
#else
#define trw  newobj::exception e_newobj;e_newobj.set_codeaddr(__LINE__,__FILE__,__FUNCTION__);throw e_newobj
#define trw_str(exp_str)  newobj::exception e_newobj(exp_str);e_newobj.set_codeaddr(__LINE__,__FILE__,__FUNCTION__);throw e_newobj
#endif

#define null_ret(ptr_p) if(ptr_p == NULL)return;
#define null_ret_f(ptr_p) if(ptr_p == NULL)return false;
#define null_trw(param) if((param) == NULL){trw;}
#define null_trw_str(param,str) if((param) == NULL){trw_str((str));}
#define t_ret(ptr_p) if(ptr_p)return
#define t_ret_t(ptr_p) if(ptr_p)return true
#define t_ret_f(ptr_p) if(ptr_p)return false	
#define t_trw(bools)  if((bools)){trw;}
#define t_trw_str(bools,str)  if((bools)){trw_str((str));}
#define t_ret_str(bools,cstr) if((bools)==true)return cstr;
#define f_trw(bools)  if((bools)==false){trw;}
#define f_trw_str(bools,str)  if((bools)==false){trw_str((str));}
#define f_ret_f(bools) if((bools)==false)return false;
#define f_ret_t(bools) if((bools)==false)return true;
#define f_ret(bools) if((bools)==false)return;
#define if_ret(bools,A,B) if((bools)==true){return A;}else{return B;}
#define ret_t(sz) sz;return true;
#define f_ret_str(bools,cstr) if((bools)==false)return cstr;
#define t_ret_var(ptr_p,var) if((ptr_p) == true)return var;
#define f_ret_var(ptr_p,var) if((ptr_p) == false)return var;
#define to_ptr(ptr_typename,ptrnum) ((ptr_typename)ptrnum)

#if defined(NEWOBJ1)
#define output_lock_printf(TYPE,MUTEX)
#else
#if defined(DEFINE_PRINT_MUTEX)
#define output_lock_printf(TYPE,MUTEX)														\
	printf("%s\n", nstring(nstring("[") + nstring(TYPE) + nstring("] [") + nstring(__FILE__) + "][" + nstring(__FUNCTION__) + "][" + nstring::from((int32)__LINE__) + "]["+nstring::from((ptr)&MUTEX)+"]").c_str())	
#else
#define output_lock_printf(TYPE,MUTEX)
#endif
#endif



#define ____lock____(MUTEX,LOCK) output_lock_printf("AutoLock Begin",MUTEX);newobj::alock lock(MUTEX,LOCK); output_lock_printf("AutoLock End",MUTEX)
#define ____lock_nolog____(MUTEX,LOCK) newobj::alock lock(MUTEX,LOCK)
#define ____melock(MUTEX) output_lock_printf("Lock Begin",MUTEX);MUTEX.lock();output_lock_printf("Lock End",MUTEX)
#define ____meunlock(MUTEX) output_lock_printf("UnLock Begin",MUTEX);MUTEX.unlock();output_lock_printf("UnLock End",MUTEX)
#ifdef NEWOBJ_STATIC
#define NEWOBJ_API
#define MNETWORK_API 
#define MQT_API 
#define MGAME_API 
#else

#define NEWOBJ_EXPORT_API __declspec(dllexport)
#ifdef EXPORT_DLL
#define C_DLL_HEADER extern "C" __declspec(dllexport)
#else
#define C_DLL_HEADER extern "C" __declspec(dllimport)
#endif
#define C_CALL __stdcall 



#ifdef EXPORT_LIB_NEWOBJ
#define NEWOBJ_API __declspec(dllexport)
#else
#define NEWOBJ_API __declspec(dllimport)
#endif
#ifdef EXPORT_MODULE_NETWORK
#define MNETWORK_API __declspec(dllexport)
#else
#define MNETWORK_API __declspec(dllimport)
#endif
#ifdef EXPORT_MODULE_QT
#define MQT_API __declspec(dllexport)
#else
#define MQT_API __declspec(dllimport)
#endif
#ifdef EXPORT_MODULE_GAME
#define MGAME_API __declspec(dllexport)
#else
#define MGAME_API __declspec(dllimport)
#endif


#endif




/*******************¹¦ÄÜºê*********************/
#define INSTANCE_BEGIN(CLASSTYPE)                                       \
    private:                                                            \
        static newobj::mutex __instance_m_mutex;                           \
        static ptr __instance_m_instance;                               \
    public:                                                             \
        static CLASSTYPE* GetInstance(){                                \
            if(__instance_m_instance != 0)                              \
                return (CLASSTYPE*)__instance_m_instance;               \
            CLASSTYPE::__instance_m_mutex.lock();                       \
            if(__instance_m_instance != 0){                             \
                CLASSTYPE::__instance_m_mutex.unlock();                 \
                return (CLASSTYPE*)__instance_m_instance;               \
            }                                                           \
            CLASSTYPE::__instance_m_instance = (ptr)new CLASSTYPE();    \
            CLASSTYPE::__instance_m_mutex.unlock();                     \
            return (CLASSTYPE*)__instance_m_instance;                   \
        }


#define INSTANCE_END(CLASSTYPE)                                         \
    newobj::mutex CLASSTYPE::__instance_m_mutex;                           \
    ptr CLASSTYPE::__instance_m_instance = 0
/****************************[ QT ]************************************/
#ifdef LIB_QT
#define QSENDER(TO_OBJ) qobject_cast<TO_OBJ>(sender())
#define QT_WAIT(TIMEOUT_MSEC,LOGIC)                                     \
            {                                                                                           \
                QTime time;                                                                     \
                    time.start();                                                                   \
                while (time.elapsed() < (int)TIMEOUT_MSEC && (LOGIC))\
                    QCoreApplication::processEvents();                              \
            }
#endif
#ifdef LIB_QT
#ifndef QT4
#define QTSTR(TEXT) QStringLiteral(TEXT)
#else
#define QTSTR(TEXT) QObject::tr(TEXT)
#endif
#endif

/****************************[ JSON ]************************************/
#define J_INT(VAR) ((int)(VAR.empty()==true?0:VAR.get<int>()))
#define J_INT64(VAR) ((uint64)(VAR.empty()==true?0:VAR.get<uint64>()))
#define J_BOOL(VAR) ((int)(VAR.empty()==true?false:VAR.get<bool>()))
#define J_STR(VAR) ((nstring)(VAR.empty()==true?nstring(""):VAR.get<nstring>()))
#define J_SIZE(VAR) nog::gen::GetJsonSie(VAR)
#define J_DUMP(VAR) VAR.empty()==true?"{}":nstring(VAR.dump())
#define J_LOOP(JSON) for_iter(iter,JSON)
#define J_ITER (*iter)
#define J_ITER_INT(CTX) J_INT(J_ITER CTX)
#define J_ITER_INT64(CTX) J_INT(J_ITER CTX)
#define J_ITER_STR(CTX) J_STR(J_ITER CTX)
#define J_ITER_SIZE(CTX) J_SIZE(J_ITER CTX)
#define J_ITER_BOOL(CTX) J_BOOL(J_ITER CTX)
#define J_ITER_DUMP(CTX) J_DUMP(J_ITER CTX)

#ifdef _WIN32
#define SPRINTF sprintf_s
#define SSCANF sscanf_s
#define STRCPY strcpy_s
#else
#define SPRINTF sprintf
#define SSCANF sscanf
#define STRCPY strcpy
#endif


#define __min__(a,b) a<b?a:b
#define __max__(a,b) a>b?a:b
#define CONT_STRING(ARG) #ARG
