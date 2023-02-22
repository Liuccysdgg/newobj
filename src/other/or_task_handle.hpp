#pragma once
namespace newobj
{
	namespace other
	{
		template<typename CONTAINER>
		class irouter;


		template<typename CONTAINER/*处理结构*/, typename SEND/*发送结构*/>
		class ihandle
		{
		public:
			ihandle(irouter<CONTAINER>* router)
			{
				m_router = router;
			}
			CONTAINER& get()
			{
				return m_router->Get();
			}
			virtual bool reply(SEND send) = 0;
			virtual bool send(SEND send,uint64 connID) = 0;
		public:
			irouter<CONTAINER>* m_router = nullptr;
		private:
			
		};
	}
}