#pragma once
#include <functional>

//暂时无法判断回调类释放安全检测
class TBaseVoidDelegate {
public:
	TBaseVoidDelegate(std::function<void()> delegate)
	{
		data = delegate;
	}
	TBaseVoidDelegate(){}
public:
	void Bind(std::function<void()>&& delegate)
	{
		data=delegate;
	}
	void Bind(std::function<void()>& delegate)
	{
		data = delegate;
	}
	void Call()
	{
		data();
	}
private:
	std::function<void()> data;
};

// template <typename TUObject, typename Tparam>
// class TBaseDelegate {
// 	typedef void (TUObject::* CbFun)(Tparam);
// // 
// // 	template <typename TUObject, typename Tparam>
// // 	struct FDelegateData
// // 	{
// // 		CbFun pCbFun;
// // 		TUObject* m_pInstance;
// // 		FDelegateData()
// // 		{
// // 			m_pInstance = nullptr;
// // 		}
// // 		~FDelegateData()
// // 		{
// // 			m_pInstance = nullptr;
// // 		}
// // 	};
// public:
// 	template <typename TUObject, typename Tparam>
// 	void Bind(TUObject* pInstance, CbFun pFun)
// 	{
// 		if (pInstance)
// 		{
// 			m_pInstance = pInstance;
// 			pCbFun = pFun;
// 		}
// 
// 	}
// 	template <typename TUObject, typename Tparam>
// 	void Call(Tparam* pParam)
// 	{
// 		if (IsBound())
// 		{
// 			(m_pInstance->*pCbFun)(pParam);
// 		}
// 	}
// 	bool IsBound()
// 	{
// 		if (m_pInstance)
// 		{
// 			return true;
// 		}
// 		return false;
// 	}
// private:                                                
// 	 	CbFun pCbFun;
//  		TUObject* m_pInstance;
// };
