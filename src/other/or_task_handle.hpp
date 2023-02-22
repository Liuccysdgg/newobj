#pragma once
namespace newobj
{
	namespace other
	{
		template<typename CONTAINER>
		class irouter;


		template<typename CONTAINER/*����ṹ*/, typename SEND/*���ͽṹ*/>
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