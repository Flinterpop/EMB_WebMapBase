// CodeGear C++Builder
// Copyright (c) 1995, 2022 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'System.Threading.pas' rev: 35.00 (Windows)

#ifndef System_ThreadingHPP
#define System_ThreadingHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <System.Types.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <System.Generics.Collections.hpp>
#include <System.Generics.Defaults.hpp>
#include <System.TimeSpan.hpp>
#include <System.SyncObjs.hpp>

//-- user supplied -----------------------------------------------------------

namespace System
{
namespace Threading
{
//-- forward type declarations -----------------------------------------------
__interface DELPHIINTERFACE TExceptionHandlerProc;
typedef System::DelphiInterface<TExceptionHandlerProc> _di_TExceptionHandlerProc;
class DELPHICLASS EAggregateException;
template<typename T> class DELPHICLASS TSparseArray__1;
template<typename T> class DELPHICLASS TWorkStealingQueue__1;
class DELPHICLASS TObjectCache;
class DELPHICLASS TObjectCaches;
class DELPHICLASS TThreadPool;
struct TThreadPoolStats;
__interface DELPHIINTERFACE ITask;
typedef System::DelphiInterface<ITask> _di_ITask;
template<typename T> __interface DELPHIINTERFACE IFuture__1;
#ifdef __clang__
template<typename T> using _di_IFuture__1 = System::DelphiInterface<IFuture__1<T>>;
#endif
class DELPHICLASS TAbstractTask;
class DELPHICLASS TTask;
class DELPHICLASS TParallel;
//-- type declarations -------------------------------------------------------
/* [Template Alias] */
#if defined(__clang__)
template<typename T> using TFunctionEvent__1 = T __fastcall (__closure *)(System::TObject* Sender);
#endif

typedef void __fastcall (__closure *TExceptionHandlerEvent)(System::Sysutils::Exception* const AException, bool &Handled);

__interface TExceptionHandlerProc  : public System::IInterface 
{
	virtual void __fastcall Invoke(System::Sysutils::Exception* const AException, bool &Handled) = 0 ;
};

#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
class PASCALIMPLEMENTATION EAggregateException : public System::Sysutils::Exception
{
	typedef System::Sysutils::Exception inherited;
	
	
public:
	class DELPHICLASS TExceptionEnumerator;
	class PASCALIMPLEMENTATION TExceptionEnumerator : public System::TObject
	{
		typedef System::TObject inherited;
		
	private:
		int FIndex;
		EAggregateException* FException;
		System::Sysutils::Exception* __fastcall GetCurrent();
		
	protected:
		__fastcall TExceptionEnumerator(EAggregateException* const AException);
		
	public:
		bool __fastcall MoveNext();
		__property System::Sysutils::Exception* Current = {read=GetCurrent};
	public:
		/* TObject.Destroy */ inline __fastcall virtual ~TExceptionEnumerator() { }
		
	};
	
	
	
public:
	System::Sysutils::Exception* operator[](int Index) { return this->InnerExceptions[Index]; }
	
private:
	static const System::Int8 NumLoggedExceptions = System::Int8(0xa);
	
#ifndef _WIN64
	System::DynamicArray<System::Sysutils::Exception*> FInnerExceptions;
#else /* _WIN64 */
	System::TArray__1<System::Sysutils::Exception*> FInnerExceptions;
#endif /* _WIN64 */
	int __fastcall GetCount();
	System::Sysutils::Exception* __fastcall GetInnerException(int Index);
	void __fastcall ExtractExceptionsToList(System::Generics::Collections::TList__1<System::Sysutils::Exception*>* const AList);
	
protected:
	__fastcall EAggregateException(const System::UnicodeString AMessage, System::Generics::Collections::TList__1<System::Sysutils::Exception*>* const AList)/* overload */;
	
public:
	__fastcall EAggregateException(System::Sysutils::Exception* const *AExceptionArray, const int AExceptionArray_High)/* overload */;
	__fastcall EAggregateException(const System::UnicodeString AMessage, System::Sysutils::Exception* const *AExceptionArray, const int AExceptionArray_High)/* overload */;
	__fastcall virtual ~EAggregateException();
	TExceptionEnumerator* __fastcall GetEnumerator();
	void __fastcall Handle(TExceptionHandlerEvent AExceptionHandlerEvent)/* overload */;
	void __fastcall Handle(const _di_TExceptionHandlerProc AExceptionHandlerProc)/* overload */;
	void __fastcall Add(System::Sysutils::Exception* AException);
	virtual System::UnicodeString __fastcall ToString();
	__property int Count = {read=GetCount, nodefault};
	__property System::Sysutils::Exception* InnerExceptions[int Index] = {read=GetInnerException/*, default*/};
public:
	/* Exception.CreateFmt */ inline __fastcall EAggregateException(const System::UnicodeString Msg, const System::TVarRec *Args, const int Args_High) : System::Sysutils::Exception(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EAggregateException(NativeUInt Ident)/* overload */ : System::Sysutils::Exception(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAggregateException(System::PResStringRec ResStringRec)/* overload */ : System::Sysutils::Exception(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAggregateException(NativeUInt Ident, const System::TVarRec *Args, const int Args_High)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EAggregateException(System::PResStringRec ResStringRec, const System::TVarRec *Args, const int Args_High)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EAggregateException(const System::UnicodeString Msg, int AHelpContext) : System::Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAggregateException(const System::UnicodeString Msg, const System::TVarRec *Args, const int Args_High, int AHelpContext) : System::Sysutils::Exception(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAggregateException(NativeUInt Ident, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAggregateException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAggregateException(System::PResStringRec ResStringRec, const System::TVarRec *Args, const int Args_High, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAggregateException(NativeUInt Ident, const System::TVarRec *Args, const int Args_High, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_High, AHelpContext) { }
	
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
// Template declaration generated by Delphi parameterized types is
// used only for accessing Delphi variables and fields.
// Don't instantiate with new type parameters in user code.
template<typename T> class PASCALIMPLEMENTATION TSparseArray__1 : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	System::TObject* FLock;
#ifndef _WIN64
	System::DynamicArray<T> FArray;
#else /* _WIN64 */
	System::TArray__1<T> FArray;
#endif /* _WIN64 */
	
public:
	__fastcall TSparseArray__1(int InitialSize);
	__fastcall virtual ~TSparseArray__1();
	int __fastcall Add(const T Item);
	void __fastcall Lock();
	void __fastcall Remove(const T Item);
	void __fastcall Unlock();
#ifndef _WIN64
	__property System::DynamicArray<T> Current = {read=FArray};
#else /* _WIN64 */
	__property System::TArray__1<T> Current = {read=FArray};
#endif /* _WIN64 */
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
// Template declaration generated by Delphi parameterized types is
// used only for accessing Delphi variables and fields.
// Don't instantiate with new type parameters in user code.
template<typename T> class PASCALIMPLEMENTATION TWorkStealingQueue__1 : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	static const System::Int8 InitialSize = System::Int8(0x20);
	
	System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<T> > FComparer;
#ifndef _WIN64
	System::DynamicArray<T> FArray;
#else /* _WIN64 */
	System::TArray__1<T> FArray;
#endif /* _WIN64 */
	int FMask;
	int FHeadIndex;
	int FTailIndex;
	System::TObject* FForeignLock;
	bool __fastcall GetIsEmpty();
	int __fastcall GetCount();
	
public:
	__fastcall TWorkStealingQueue__1();
	__fastcall virtual ~TWorkStealingQueue__1();
	bool __fastcall LocalFindAndRemove(const T AItem);
	void __fastcall LocalPush(const T AItem);
	bool __fastcall LocalPop(/* out */ T &AItem);
	bool __fastcall TrySteal(/* out */ T &AItem, unsigned Timeout = (unsigned)(0x0));
	bool __fastcall Remove(const T AItem);
	__property bool IsEmpty = {read=GetIsEmpty, nodefault};
	__property int Count = {read=GetCount, nodefault};
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
class PASCALIMPLEMENTATION TObjectCache : public System::TObject
{
	typedef System::TObject inherited;
	
	
private:
	struct TCacheEntry;
	typedef TCacheEntry *PCacheEntry;
	
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	struct DECLSPEC_DRECORD TCacheEntry
	{
	public:
		TObjectCache::TCacheEntry *Next;
	};
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	
	
public:
	static const System::Int8 CObjectCacheLimit = System::Int8(0x32);
	
	
private:
	TCacheEntry *FRoot;
	int FCount;
	System::TClass FClassType;
	void __fastcall Push(PCacheEntry &Stack, PCacheEntry EventItem);
	PCacheEntry __fastcall Pop(PCacheEntry &Stack);
	
public:
	__fastcall TObjectCache(System::TClass AClass);
	__fastcall virtual ~TObjectCache();
	bool __fastcall Insert(void * Instance);
	void * __fastcall Remove();
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

class PASCALIMPLEMENTATION TObjectCaches : public System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>
{
	typedef System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*> inherited;
	
public:
	void __fastcall AddObjectCache(System::TClass AClass);
public:
	/* {System_Generics_Collections}TObjectDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(System::Generics::Collections::TDictionaryOwnerships Ownerships, int ACapacity)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(Ownerships, ACapacity) { }
	/* {System_Generics_Collections}TObjectDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(System::Generics::Collections::TDictionaryOwnerships Ownerships, const System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<System::TClass> > AComparer)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(Ownerships, AComparer) { }
	/* {System_Generics_Collections}TObjectDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(System::Generics::Collections::TDictionaryOwnerships Ownerships, int ACapacity, const System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<System::TClass> > AComparer)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(Ownerships, ACapacity, AComparer) { }
	
public:
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches()/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>() { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(int ACapacity)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(ACapacity) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(const System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<System::TClass> > AComparer)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(AComparer) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(int ACapacity, const System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<System::TClass> > AComparer)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(ACapacity, AComparer) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(System::Generics::Collections::TEnumerable__1<System::Generics::Collections::TPair__2<System::TClass,TObjectCache*> >* const Collection)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(Collection) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(System::Generics::Collections::TEnumerable__1<System::Generics::Collections::TPair__2<System::TClass,TObjectCache*> >* const Collection, const System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<System::TClass> > AComparer)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(Collection, AComparer) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(const System::Generics::Collections::TPair__2<System::TClass,TObjectCache*> *AItems, const int AItems_High)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(AItems, AItems_High) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Create */ inline __fastcall TObjectCaches(const System::Generics::Collections::TPair__2<System::TClass,TObjectCache*> *AItems, const int AItems_High, const System::DelphiInterface<System::Generics::Defaults::IEqualityComparer__1<System::TClass> > AComparer)/* overload */ : System::Generics::Collections::TObjectDictionary__2<System::TClass,TObjectCache*>(AItems, AItems_High, AComparer) { }
	/* {System_Generics_Collections}TDictionary<System_TClass,System_Threading_TObjectCache>.Destroy */ inline __fastcall virtual ~TObjectCaches() { }
	
};


class PASCALIMPLEMENTATION TThreadPool : /*[[sealed]]*/ public System::TObject
{
	typedef System::TObject inherited;
	friend class DELPHICLASS TTask;
	friend class /* __interface */ ITask;
	friend class DELPHICLASS TAbstractTask;
	
	
public:
	__interface DELPHIINTERFACE IControlFlag;
	typedef System::DelphiInterface<IControlFlag> _di_IControlFlag;
	__interface IControlFlag  : public System::IInterface 
	{
		virtual int __fastcall Increment() = 0 ;
		virtual int __fastcall Value() = 0 ;
	};
	
	
protected:
	struct DECLSPEC_DRECORD TSafeSharedInteger
	{
	private:
		int *FSharedVar;
		int __fastcall GetInteger();
		void __fastcall SetInteger(const int Value);
		
	public:
		__fastcall TSafeSharedInteger(int &SharedVar);
		int __fastcall Increment();
		int __fastcall Decrement();
		int __fastcall CompareExchange(int Value, int Comparand);
		// explicit __fastcall operator int();
		__property int Value = {read=GetInteger, write=SetInteger};
		TSafeSharedInteger() {}
	};
	
	
	struct DECLSPEC_DRECORD TSafeSharedUInt64
	{
	private:
		unsigned __int64 *FSharedVar;
		unsigned __int64 __fastcall GetUInt64();
		void __fastcall SetUInt64(const unsigned __int64 Value);
		
	public:
		__fastcall TSafeSharedUInt64(unsigned __int64 &SharedVar);
		unsigned __int64 __fastcall Increment();
		unsigned __int64 __fastcall Decrement();
		// explicit __fastcall operator unsigned __int64();
		__property unsigned __int64 Value = {read=GetUInt64, write=SetUInt64};
		TSafeSharedUInt64() {}
	};
	
	
	__interface DELPHIINTERFACE IThreadPoolWorkItem;
	typedef System::DelphiInterface<IThreadPoolWorkItem> _di_IThreadPoolWorkItem;
	__interface IThreadPoolWorkItem  : public System::IInterface 
	{
		virtual bool __fastcall ShouldExecute() = 0 ;
		virtual void __fastcall ExecuteWork() = 0 ;
	};
	
	class DELPHICLASS TControlFlag;
#ifndef _WIN64
	#pragma pack(push,4)
#endif /* not _WIN64 */
	class PASCALIMPLEMENTATION TControlFlag : public System::TInterfacedObject
	{
		typedef System::TInterfacedObject inherited;
		
	private:
		int FControlFlag;
		
	private:
		int __fastcall Increment();
		int __fastcall Value();
		
	public:
		__fastcall TControlFlag()/* overload */;
	public:
		/* TObject.Destroy */ inline __fastcall virtual ~TControlFlag() { }
		
private:
		void *__TThreadPool_IControlFlag;	// TThreadPool::IControlFlag 
		
public:
		operator TThreadPool::IControlFlag*(void) { return (TThreadPool::IControlFlag*)&__TThreadPool_IControlFlag; }
		
	};
	
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	class DELPHICLASS TAbstractWorkerData;
#ifndef _WIN64
	#pragma pack(push,4)
#endif /* not _WIN64 */
	class PASCALIMPLEMENTATION TAbstractWorkerData : public System::TInterfacedObject
	{
		typedef System::TInterfacedObject inherited;
		
	protected:
		TThreadPool::_di_IControlFlag FControlFlag;
		bool __fastcall ShouldExecute();
		
	public:
		__classmethod virtual System::TObject* __fastcall NewInstance();
		virtual void __fastcall FreeInstance();
	public:
		/* TObject.Create */ inline __fastcall TAbstractWorkerData() : System::TInterfacedObject() { }
		/* TObject.Destroy */ inline __fastcall virtual ~TAbstractWorkerData() { }
		
	};
	
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	class DELPHICLASS TWorkerData;
	class PASCALIMPLEMENTATION TWorkerData : public TThreadPool::TAbstractWorkerData
	{
		typedef TThreadPool::TAbstractWorkerData inherited;
		
	private:
		// __classmethod void __fastcall Create@();
		
	protected:
		System::TObject* FSender;
		System::Classes::TNotifyEvent FWorkerEvent;
		System::Sysutils::_di_TProc FProc;
		void __fastcall ExecuteWork();
		
	private:
		// __classmethod void __fastcall Destroy@();
	public:
		/* TObject.Create */ inline __fastcall TWorkerData() : TThreadPool::TAbstractWorkerData() { }
		/* TObject.Destroy */ inline __fastcall virtual ~TWorkerData() { }
		
private:
		void *__TThreadPool_IThreadPoolWorkItem;	// TThreadPool::IThreadPoolWorkItem 
		
public:
		operator TThreadPool::IThreadPoolWorkItem*(void) { return (TThreadPool::IThreadPoolWorkItem*)&__TThreadPool_IThreadPoolWorkItem; }
		
	};
	
	
	class DELPHICLASS TBaseWorkerThread;
	class PASCALIMPLEMENTATION TBaseWorkerThread : public System::Classes::TThread
	{
		typedef System::Classes::TThread inherited;
		
	private:
		TThreadPool* FThreadPool;
		System::Syncobjs::TLightweightEvent* FRunningEvent;
		static int WorkerThreadID;
		
	protected:
		void __fastcall SafeTerminate();
		virtual void __fastcall Execute();
		__property TThreadPool* ThreadPool = {read=FThreadPool};
		__property System::Syncobjs::TLightweightEvent* RunningEvent = {read=FRunningEvent};
		
	public:
		__fastcall TBaseWorkerThread(TThreadPool* AThreadPool);
		__fastcall virtual ~TBaseWorkerThread();
		virtual void __fastcall BeforeDestruction();
	};
	
	
	class DELPHICLASS TQueueWorkerThread;
	class PASCALIMPLEMENTATION TQueueWorkerThread : public TThreadPool::TBaseWorkerThread
	{
		typedef TThreadPool::TBaseWorkerThread inherited;
		
	private:
		TWorkStealingQueue__1<TThreadPool::_di_IThreadPoolWorkItem>* FWorkQueue;
		TThreadPool::TSafeSharedInteger FThreadSuspended;
		TThreadPool::TSafeSharedUInt64 FLastSuspendTick;
		System::Syncobjs::TLightweightEvent* FRetiredThreadWakeEvent;
		
	protected:
		bool __fastcall SuspendWork();
		bool __fastcall TryToRetire();
		void __fastcall ExecuteWorkItem(TThreadPool::_di_IThreadPoolWorkItem &Item);
		virtual void __fastcall Execute();
		void __fastcall PushLocalWorkToGlobal();
		__property TWorkStealingQueue__1<TThreadPool::_di_IThreadPoolWorkItem>* WorkQueue = {read=FWorkQueue};
		
	public:
		__fastcall TQueueWorkerThread(TThreadPool* AThreadPool);
		__fastcall virtual ~TQueueWorkerThread();
	};
	
	
	class DELPHICLASS TThreadPoolMonitor;
	class PASCALIMPLEMENTATION TThreadPoolMonitor : public System::Classes::TThread
	{
		typedef System::Classes::TThread inherited;
		
	private:
		TThreadPool* FThreadPool;
		
	protected:
		bool __fastcall IsThrottledDelay(unsigned __int64 LastCreationTick, unsigned ThreadCount);
		virtual void __fastcall Execute();
		void __fastcall GrowThreadPoolIfStarved();
		
	public:
		__fastcall TThreadPoolMonitor(TThreadPool* AThreadPool);
	public:
		/* TThread.Destroy */ inline __fastcall virtual ~TThreadPoolMonitor() { }
		
	};
	
	
	
private:
	enum class DECLSPEC_DENUM TMonitorThreadStat : unsigned char { Created, NoWorkers, ForceSize = 31 };
	
	typedef System::Set<TMonitorThreadStat, _DELPHI_SET_ENUMERATOR(TMonitorThreadStat::Created), _DELPHI_SET_ENUMERATOR(TMonitorThreadStat::ForceSize)> TMonitorThreadStatus;
	
	
protected:
	static DELPHITHREAD TQueueWorkerThread* QueueThread;
	
private:
	static const System::Int8 MaxThreadsPerCPU = System::Int8(0x2);
	
	static const System::Int8 CPUUsageHigh = System::Int8(0x5f);
	
	static const System::Int8 CPUUsageLow = System::Int8(0x50);
	
	static const System::Int8 CPUUsageLowest = System::Int8(0x14);
	
	static const System::Int8 NumCPUUsageSamples = System::Int8(0xa);
	
	static const System::Word MonitorThreadDelay = System::Word(0x1f4);
	
	static const System::Word SuspendInterval = System::Word(0x157c);
	
	static const System::Word SuspendTime = System::Word(0x258);
	
	static const System::Word RetirementDelay = System::Word(0x1388);
	
	System::Generics::Collections::TQueue__1<_di_IThreadPoolWorkItem>* FQueue;
	TSparseArray__1<TWorkStealingQueue__1<_di_IThreadPoolWorkItem>*>* FQueues;
	System::Generics::Collections::TThreadList__1<TBaseWorkerThread*>* FThreads;
	int FWorkerThreadCount;
	int FMinLimitWorkerThreadCount;
	int FMaxLimitWorkerThreadCount;
	int FIdleWorkerThreadCount;
	int FQueuedRequestCount;
	int FRetiredWorkerThreadCount;
	TMonitorThreadStatus FMonitorThreadStatus;
	int FAverageCPUUsage;
	int FCurrentCPUUsage;
	int FThreadSuspended;
	unsigned __int64 FLastSuspendTick;
	System::Syncobjs::TLightweightEvent* FRetiredThreadWakeEvent;
	unsigned __int64 FLastThreadCreationTick;
	int FLastQueuedRequestCount;
	bool FShutdown;
	static TThreadPool* FDefaultPool;
	static TObjectCaches* FObjectCaches;
	TBaseWorkerThread* __fastcall CreateWorkerThread();
	void __fastcall CreateMonitorThread();
	void __fastcall WaitMonitorThread();
	void __fastcall DecIdleThreadCount();
	void __fastcall DecWorkRequestCount();
	void __fastcall GrowWorkerPool();
	void __fastcall IncIdleThreadCount();
	void __fastcall IncWorkRequestCount();
	void __fastcall ResurrectRetiredThread();
	bool __fastcall ShouldGrowPool();
	void __fastcall QueueWorkItem(const _di_IThreadPoolWorkItem WorkerData, bool UseLocalQueue)/* overload */;
	bool __fastcall TryRemoveWorkItem(const _di_IThreadPoolWorkItem WorkerData);
	int __fastcall GetMaxWorkerThreads();
	int __fastcall GetMinWorkerThreads();
	static TThreadPool* __fastcall GetCurrentThreadPool();
	static TObjectCaches* __fastcall GetObjectCaches();
	
private:
	// __classmethod void __fastcall Create@();
	// __classmethod void __fastcall Destroy@();
	
private:
	/* static */ __property TObjectCaches* ObjectCaches = {read=GetObjectCaches};
	
public:
	__fastcall TThreadPool();
	__fastcall virtual ~TThreadPool();
	static _di_IControlFlag __fastcall NewControlFlag();
	void __fastcall QueueWorkItem(System::TObject* Sender, System::Classes::TNotifyEvent WorkerEvent, const _di_IControlFlag AControlFlag = _di_IControlFlag())/* overload */;
	void __fastcall QueueWorkItem(const System::Sysutils::_di_TProc WorkerEvent, const _di_IControlFlag AControlFlag = _di_IControlFlag())/* overload */;
	bool __fastcall SetMaxWorkerThreads(int Value);
	bool __fastcall SetMinWorkerThreads(int Value);
	__property int MaxWorkerThreads = {read=GetMaxWorkerThreads, nodefault};
	__property int MinWorkerThreads = {read=GetMinWorkerThreads, nodefault};
	/* static */ __property TThreadPool* Default = {read=FDefaultPool};
	/* static */ __property TThreadPool* Current = {read=GetCurrentThreadPool};
};


struct DECLSPEC_DRECORD TThreadPoolStats
{
private:
	int FWorkerThreadCount;
	int FMinLimitWorkerThreadCount;
	int FMaxLimitWorkerThreadCount;
	int FIdleWorkerThreadCount;
	int FQueuedRequestCount;
	int FRetiredWorkerThreadCount;
	int FAverageCPUUsage;
	int FCurrentCPUUsage;
	int FThreadSuspended;
	unsigned __int64 FLastSuspendTick;
	unsigned __int64 FLastThreadCreationTick;
	int FLastQueuedRequestCount;
	static TThreadPoolStats __fastcall GetCurrent();
	static TThreadPoolStats __fastcall GetDefault();
	
public:
	__property int WorkerThreadCount = {read=FWorkerThreadCount};
	__property int MinLimitWorkerThreadCount = {read=FMinLimitWorkerThreadCount};
	__property int MaxLimitWorkerThreadCount = {read=FMaxLimitWorkerThreadCount};
	__property int IdleWorkerThreadCount = {read=FIdleWorkerThreadCount};
	__property int QueuedRequestCount = {read=FQueuedRequestCount};
	__property int RetiredWorkerThreadCount = {read=FRetiredWorkerThreadCount};
	__property int AverageCPUUsage = {read=FAverageCPUUsage};
	__property int CurrentCPUUsage = {read=FCurrentCPUUsage};
	__property int ThreadSuspended = {read=FThreadSuspended};
	__property unsigned __int64 LastSuspendTick = {read=FLastSuspendTick};
	__property unsigned __int64 LastThreadCreationTick = {read=FLastThreadCreationTick};
	__property int LastQueuedRequestCount = {read=FLastQueuedRequestCount};
	static TThreadPoolStats __fastcall Get(TThreadPool* const Pool);
	/* static */ __property TThreadPoolStats Current = {read=GetCurrent};
	/* static */ __property TThreadPoolStats Default = {read=GetDefault};
};


enum class DECLSPEC_DENUM TTaskStatus : unsigned char { Created, WaitingToRun, Running, Completed, WaitingForChildren, Canceled, Exception };

__interface ITask  : public TThreadPool::IThreadPoolWorkItem 
{
	virtual bool __fastcall Wait(unsigned Timeout = (unsigned)(0xffffffff)) = 0 /* overload */;
	virtual bool __fastcall Wait(const System::Timespan::TTimeSpan &Timeout) = 0 /* overload */;
	virtual void __fastcall Cancel() = 0 ;
	virtual void __fastcall CheckCanceled() = 0 ;
	virtual _di_ITask __fastcall Start() = 0 ;
	virtual TTaskStatus __fastcall GetStatus() = 0 ;
	virtual int __fastcall GetId() = 0 ;
	__property int Id = {read=GetId};
	__property TTaskStatus Status = {read=GetStatus};
};

// Template declaration generated by Delphi parameterized types is
// used only for accessing Delphi variables and fields.
// Don't instantiate with new type parameters in user code.
template<typename T> __interface IFuture__1  : public ITask 
{
	HIDESBASE virtual System::DelphiInterface<IFuture__1<T> > __fastcall StartFuture() = 0;
	virtual T __fastcall GetValue() = 0 ;
	__property T Value = {read=GetValue};
};

#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
class PASCALIMPLEMENTATION TAbstractTask : public TThreadPool::TAbstractWorkerData
{
	typedef TThreadPool::TAbstractWorkerData inherited;
	
	
protected:
	__interface DELPHIINTERFACE IInternalTask;
	typedef System::DelphiInterface<IInternalTask> _di_IInternalTask;
	__interface IInternalTask  : public ITask 
	{
		virtual void __fastcall AddCompleteEvent(const System::DelphiInterface<System::Sysutils::TProc__1<_di_ITask> > Proc) = 0 ;
		virtual void __fastcall HandleChildCompletion(const TAbstractTask::_di_IInternalTask Task) = 0 ;
		virtual void __fastcall SetExceptionObject(System::TObject* const Exception) = 0 ;
		virtual void __fastcall RemoveCompleteEvent(const System::DelphiInterface<System::Sysutils::TProc__1<_di_ITask> > Proc) = 0 ;
		virtual TThreadPool::_di_IControlFlag __fastcall GetControlFlag() = 0 ;
	};
	
	
public:
	/* TObject.Create */ inline __fastcall TAbstractTask() : TThreadPool::TAbstractWorkerData() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TAbstractTask() { }
	
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

class PASCALIMPLEMENTATION TTask : public TAbstractTask
{
	typedef TAbstractTask inherited;
	
	
protected:
	enum class DECLSPEC_DENUM TOptionStateFlag : unsigned char { Started, CallbackRun, ChildWait, Complete, Canceled, Faulted, Replicating, Replica, Raised, Destroying, ForceSize = 31 };
	
	typedef System::Set<TOptionStateFlag, _DELPHI_SET_ENUMERATOR(TOptionStateFlag::Started), _DELPHI_SET_ENUMERATOR(TOptionStateFlag::ForceSize)> TOptionStateFlags;
	
	enum class DECLSPEC_DENUM TCreateFlag : unsigned char { Replicating, Replica };
	
	typedef System::Set<TCreateFlag, _DELPHI_SET_ENUMERATOR(TCreateFlag::Replicating), _DELPHI_SET_ENUMERATOR(TCreateFlag::Replica)> TCreateFlags;
	
	
private:
	typedef System::Generics::Collections::TList__1<System::DelphiInterface<System::Sysutils::TProc__1<_di_ITask> > >* TCompleteEvents;
	
	struct DECLSPEC_DRECORD TUnsafeTask
	{
	private:
		TTask* FTask;
		
	public:
		static TTask::TUnsafeTask __fastcall _op_Implicit(TTask* const ATask);
		// explicit __fastcall operator TTask*();
		static TTask::TUnsafeTask __fastcall _op_Explicit(const _di_ITask ATask);
#ifndef _WIN64
		static bool __fastcall _op_Equality(const TTask::TUnsafeTask Left, const TTask::TUnsafeTask Right);
#else /* _WIN64 */
		static bool __fastcall _op_Equality(const TTask::TUnsafeTask &Left, const TTask::TUnsafeTask &Right);
#endif /* _WIN64 */
		__property TTask* Value = {read=FTask, write=FTask};
		
		TTask::TUnsafeTask& operator =(TTask* const ATask) { *this = TTask::TUnsafeTask::_op_Implicit(ATask); return *this; }
#ifndef _WIN64
		friend bool operator ==(const TTask::TUnsafeTask Left, const TTask::TUnsafeTask Right) { return TUnsafeTask::_op_Equality(Left, Right); }
#else /* _WIN64 */
		friend bool operator ==(const TTask::TUnsafeTask &Left, const TTask::TUnsafeTask &Right) { return TUnsafeTask::_op_Equality(Left, Right); }
#endif /* _WIN64 */
	};
	
	
	
protected:
	#define TTask_StateFlagMask (System::Set<TOptionStateFlag, _DELPHI_SET_ENUMERATOR(TOptionStateFlag::Started), _DELPHI_SET_ENUMERATOR(TOptionStateFlag::ForceSize)>() << TOptionStateFlag::Started << TOptionStateFlag::CallbackRun << TOptionStateFlag::ChildWait << TOptionStateFlag::Complete << TOptionStateFlag::Canceled << TOptionStateFlag::Faulted )
	
	#define TTask_OptionFlagMask (System::Set<TOptionStateFlag, _DELPHI_SET_ENUMERATOR(TOptionStateFlag::Started), _DELPHI_SET_ENUMERATOR(TOptionStateFlag::ForceSize)>() << TOptionStateFlag::Replicating << TOptionStateFlag::Replica )
	
	
private:
	TOptionStateFlags FState;
	int FTaskCountdown;
	TAbstractTask::_di_IInternalTask FParentTask;
	TThreadPool::_di_IControlFlag FParentControlFlag;
	System::Syncobjs::TLightweightEvent* FDoneEvent;
	System::Generics::Collections::TList__1<TAbstractTask::_di_IInternalTask>* FFaultedChildren;
	System::TObject* FCompleteEvents;
	int FId;
	bool FDestroying;
	static DELPHITHREAD TTask* FCurrentTask;
	static System::TObject* CompletedFlag;
	static int TaskId;
	
private:
	// __classmethod void __fastcall Create@();
	// __classmethod void __fastcall Destroy@();
	
private:
	System::Syncobjs::TLightweightEvent* __fastcall GetDoneEvent();
	static unsigned __fastcall TimespanToMilliseconds(const System::Timespan::TTimeSpan &Timeout);
	bool __fastcall InternalAddCompleteEvent(const System::DelphiInterface<System::Sysutils::TProc__1<_di_ITask> > Proc);
	
protected:
	TThreadPool* FPool;
	System::TObject* FSender;
	System::TObject* FException;
	System::Classes::TNotifyEvent FEvent;
	System::Sysutils::_di_TProc FProc;
	bool __fastcall UpdateStateAtomic(TOptionStateFlags NewState, TOptionStateFlags InvalidStates)/* overload */;
	bool __fastcall UpdateStateAtomic(TOptionStateFlags NewState, TOptionStateFlags InvalidStates, /* out */ TOptionStateFlags &OldState)/* overload */;
	void __fastcall SetTaskStop();
	virtual bool __fastcall ShouldCreateReplica();
	virtual TTask* __fastcall CreateReplicaTask(const System::Sysutils::_di_TProc AProc, TTask* AParent, TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag);
	void __fastcall CheckFaulted();
	void __fastcall SetComplete();
	void __fastcall AddChild();
	void __fastcall ForgetChild();
	bool __fastcall InternalExecuteNow();
	System::Sysutils::Exception* __fastcall GetExceptionObject();
	bool __fastcall GetIsComplete();
	bool __fastcall GetIsReplicating();
	bool __fastcall GetHasExceptions();
	bool __fastcall GetIsCanceled();
	bool __fastcall GetIsQueued();
	bool __fastcall GetWasExceptionRaised();
	virtual void __fastcall QueueEvents();
	void __fastcall Complete(bool UserEventRan);
	void __fastcall IntermediateCompletion();
	void __fastcall FinalCompletion();
	virtual void __fastcall ProcessCompleteEvents();
	void __fastcall SetRaisedState();
	bool __fastcall InternalWork(bool CheckExecuting);
	void __fastcall InternalExecute(TTask* &CurrentTaskVar);
	void __fastcall Execute();
	void __fastcall ExecuteReplicates(TTask* const Root);
	void __fastcall CallUserCode();
	void __fastcall HandleException(const _di_ITask ChildTask, System::TObject* const Exception);
	bool __fastcall MarkAsStarted();
	bool __fastcall TryExecuteNow(bool WasQueued);
	void __fastcall ExecuteWork();
	bool __fastcall Wait(unsigned Timeout = (unsigned)(0xffffffff))/* overload */;
	bool __fastcall Wait(const System::Timespan::TTimeSpan &Timeout)/* overload */;
	void __fastcall Cancel();
	void __fastcall CheckCanceled();
	_di_ITask __fastcall Start();
	int __fastcall GetId();
	TTaskStatus __fastcall GetStatus();
	void __fastcall AddCompleteEvent(const System::DelphiInterface<System::Sysutils::TProc__1<_di_ITask> > Proc);
	void __fastcall HandleChildCompletion(const TAbstractTask::_di_IInternalTask Task);
	void __fastcall SetExceptionObject(System::TObject* const Exception);
	void __fastcall RemoveCompleteEvent(const System::DelphiInterface<System::Sysutils::TProc__1<_di_ITask> > Proc);
	TThreadPool::_di_IControlFlag __fastcall GetControlFlag();
	__property bool IsComplete = {read=GetIsComplete, nodefault};
	__property bool IsReplicating = {read=GetIsReplicating, nodefault};
	__property bool HasExceptions = {read=GetHasExceptions, nodefault};
	__property bool IsCanceled = {read=GetIsCanceled, nodefault};
	__property bool IsQueued = {read=GetIsQueued, nodefault};
	__property bool WasExceptionRaised = {read=GetWasExceptionRaised, nodefault};
	__property System::Syncobjs::TLightweightEvent* DoneEvent = {read=GetDoneEvent};
	__property TThreadPool* ThreadPool = {read=FPool};
	__fastcall TTask(System::TObject* Sender, System::Classes::TNotifyEvent Event, const System::Sysutils::_di_TProc AProc, TThreadPool* const APool, TTask* const AParent, TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag)/* overload */;
	static bool __fastcall DoWaitForAll(const _di_ITask *Tasks, const int Tasks_High, unsigned Timeout);
	static int __fastcall DoWaitForAny(const _di_ITask *Tasks, const int Tasks_High, unsigned Timeout);
	static int __fastcall NewId();
	
public:
	static _di_ITask __fastcall CurrentTask();
	__fastcall TTask()/* overload */;
	__fastcall virtual ~TTask();
	static _di_ITask __fastcall Create(System::TObject* Sender, System::Classes::TNotifyEvent Event)/* overload */;
	static _di_ITask __fastcall Create(const System::Sysutils::_di_TProc Proc)/* overload */;
	static _di_ITask __fastcall Create(System::TObject* Sender, System::Classes::TNotifyEvent Event, TThreadPool* const APool)/* overload */;
	static _di_ITask __fastcall Create(const System::Sysutils::_di_TProc Proc, TThreadPool* APool)/* overload */;
#ifndef _WIN64
	template<typename T> static System::DelphiInterface<IFuture__1<T> > __fastcall Future(System::TObject* Sender, T __fastcall (__closure *)(System::TObject* Sender) )/* overload */;
	template<typename T> static System::DelphiInterface<IFuture__1<T> > __fastcall Future(System::TObject* Sender, T __fastcall (__closure *)(System::TObject* Sender) , TThreadPool* APool)/* overload */;
#else /* _WIN64 */
	template<typename T> static System::DelphiInterface<IFuture__1<T> > __fastcall Future(System::TObject* Sender, TFunctionEvent__1<T> Event)/* overload */;
	template<typename T> static System::DelphiInterface<IFuture__1<T> > __fastcall Future(System::TObject* Sender, TFunctionEvent__1<T> Event, TThreadPool* APool)/* overload */;
#endif /* _WIN64 */
	template<typename T> static System::DelphiInterface<IFuture__1<T> > __fastcall Future(const System::DelphiInterface<System::Sysutils::TFunc__1<T> > Func)/* overload */;
	template<typename T> static System::DelphiInterface<IFuture__1<T> > __fastcall Future(const System::DelphiInterface<System::Sysutils::TFunc__1<T> > Func, TThreadPool* APool)/* overload */;
	static _di_ITask __fastcall Run(System::TObject* Sender, System::Classes::TNotifyEvent Event)/* overload */;
	static _di_ITask __fastcall Run(System::TObject* Sender, System::Classes::TNotifyEvent Event, TThreadPool* APool)/* overload */;
	static _di_ITask __fastcall Run(const System::Sysutils::_di_TProc Func)/* overload */;
	static _di_ITask __fastcall Run(const System::Sysutils::_di_TProc Func, TThreadPool* APool)/* overload */;
	static bool __fastcall WaitForAll(const _di_ITask *Tasks, const int Tasks_High)/* overload */;
	static bool __fastcall WaitForAll(const _di_ITask *Tasks, const int Tasks_High, unsigned Timeout)/* overload */;
	static bool __fastcall WaitForAll(const _di_ITask *Tasks, const int Tasks_High, const System::Timespan::TTimeSpan &Timeout)/* overload */;
	static int __fastcall WaitForAny(const _di_ITask *Tasks, const int Tasks_High)/* overload */;
	static int __fastcall WaitForAny(const _di_ITask *Tasks, const int Tasks_High, unsigned Timeout)/* overload */;
	static int __fastcall WaitForAny(const _di_ITask *Tasks, const int Tasks_High, const System::Timespan::TTimeSpan &Timeout)/* overload */;
private:
	void *__TAbstractTask_IInternalTask;	// TAbstractTask::IInternalTask 
	
public:
	operator TAbstractTask::IInternalTask*(void) { return (TAbstractTask::IInternalTask*)&__TAbstractTask_IInternalTask; }
	operator ITask*(void) { return (ITask*)&__TAbstractTask_IInternalTask; }
	operator TThreadPool::IThreadPoolWorkItem*(void) { return (TThreadPool::IThreadPoolWorkItem*)&__TAbstractTask_IInternalTask; }
	
};



#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
class PASCALIMPLEMENTATION TParallel : /*[[sealed]]*/ public System::TObject
{
	typedef System::TObject inherited;
	
	
public:
	class DELPHICLASS TLoopState;
	class PASCALIMPLEMENTATION TLoopState : public System::TObject
	{
		typedef System::TObject inherited;
		
		
	private:
		enum class DECLSPEC_DENUM TLoopStateFlags : unsigned char { Exception, Broken, Stopped, Cancelled, ForceSize = 31 };
		
		typedef System::Set<TLoopStateFlags, _DELPHI_SET_ENUMERATOR(TLoopStateFlags::Exception), _DELPHI_SET_ENUMERATOR(TLoopStateFlags::ForceSize)> TLoopStateFlagSet;
		
		
	public:
		class DELPHICLASS TLoopStateFlag;
		class PASCALIMPLEMENTATION TLoopStateFlag : public System::TObject
		{
			typedef System::TObject inherited;
			
		private:
			TParallel::TLoopState::TLoopStateFlagSet FLoopStateFlags;
			bool __fastcall AtomicUpdate(TParallel::TLoopState::TLoopStateFlagSet NewState, TParallel::TLoopState::TLoopStateFlagSet InvalidStates)/* overload */;
			bool __fastcall AtomicUpdate(TParallel::TLoopState::TLoopStateFlagSet NewState, TParallel::TLoopState::TLoopStateFlagSet InvalidStates, TParallel::TLoopState::TLoopStateFlagSet &OldState)/* overload */;
			void __fastcall SetFaulted();
			void __fastcall Stop();
			
		protected:
			TParallel::TLoopState::TLoopStateFlagSet __fastcall GetLoopStateFlags();
			
		protected:
			__property TParallel::TLoopState::TLoopStateFlagSet LoopStateFlags = {read=GetLoopStateFlags, nodefault};
		public:
			/* TObject.Create */ inline __fastcall TLoopStateFlag() : System::TObject() { }
			/* TObject.Destroy */ inline __fastcall virtual ~TLoopStateFlag() { }
			
		};
		
		
		
	private:
		TLoopStateFlag* FLoopStateFlags;
		
	protected:
		__fastcall TLoopState(TLoopStateFlag* ALoopStateFlags);
		bool __fastcall GetStopped();
		bool __fastcall GetFaulted();
		System::Variant __fastcall GetLowestBreakIteration();
		virtual void __fastcall DoBreak();
		virtual bool __fastcall DoShouldExit();
		virtual System::Variant __fastcall DoGetLowestBreakIteration();
		
	public:
		void __fastcall Break();
		void __fastcall Stop();
		bool __fastcall ShouldExit();
		__property bool Faulted = {read=GetFaulted, nodefault};
		__property bool Stopped = {read=GetStopped, nodefault};
		__property System::Variant LowestBreakIteration = {read=GetLowestBreakIteration};
	public:
		/* TObject.Destroy */ inline __fastcall virtual ~TLoopState() { }
		
	};
	
	
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	struct DECLSPEC_DRECORD TLoopResult
	{
	private:
		bool FCompleted;
		System::Variant FLowestBreakIteration;
		
	public:
		__property bool Completed = {read=FCompleted};
		__property System::Variant LowestBreakIteration = {read=FLowestBreakIteration};
	};
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	
	typedef void __fastcall (__closure *TIteratorEvent)(System::TObject* Sender, int AIndex);
	
	typedef void __fastcall (__closure *TIteratorStateEvent)(System::TObject* Sender, int AIndex, TLoopState* const LoopState);
	
	typedef void __fastcall (__closure *TIteratorEvent64)(System::TObject* Sender, __int64 AIndex);
	
	typedef void __fastcall (__closure *TIteratorStateEvent64)(System::TObject* Sender, __int64 AIndex, TLoopState* const LoopState);
	
	
private:
	class DELPHICLASS TLoopState32;
	class PASCALIMPLEMENTATION TLoopState32 : /*[[sealed]]*/ public TParallel::TLoopState
	{
		typedef TParallel::TLoopState inherited;
		
		
	private:
		class DELPHICLASS TLoopStateFlag32;
		class PASCALIMPLEMENTATION TLoopStateFlag32 : public TParallel::TLoopState::TLoopStateFlag
		{
			typedef TParallel::TLoopState::TLoopStateFlag inherited;
			
		private:
			int FLowestBreakIter;
			int __fastcall GetLowestBreakIter();
			
		protected:
			__fastcall TLoopStateFlag32();
			bool __fastcall ShouldExit(int ThisIter)/* overload */;
			bool __fastcall ShouldExit()/* overload */;
			__property int LowestBreakIter = {read=GetLowestBreakIter, nodefault};
		public:
			/* TObject.Destroy */ inline __fastcall virtual ~TLoopStateFlag32() { }
			
		};
		
		
		
	protected:
		virtual void __fastcall DoBreak();
		virtual bool __fastcall DoShouldExit();
		virtual System::Variant __fastcall DoGetLowestBreakIteration();
		
	private:
		TLoopStateFlag32* FSharedLoopStateFlags;
		int FCurrentIteration;
		
	public:
		__fastcall TLoopState32(TLoopStateFlag32* ALoopStateFlags);
		__property int CurrentIteration = {read=FCurrentIteration, write=FCurrentIteration, nodefault};
	public:
		/* TObject.Destroy */ inline __fastcall virtual ~TLoopState32() { }
		
	};
	
	
	class DELPHICLASS TLoopState64;
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	class PASCALIMPLEMENTATION _ALIGNAS(8) TLoopState64 : /*[[sealed]]*/ public TParallel::TLoopState
	{
		typedef TParallel::TLoopState inherited;
		
		
	private:
		class DELPHICLASS TLoopStateFlag64;
		class PASCALIMPLEMENTATION _ALIGNAS(8) TLoopStateFlag64 : public TParallel::TLoopState::TLoopStateFlag
		{
			typedef TParallel::TLoopState::TLoopStateFlag inherited;
			
		private:
			_ALIGNAS(8) __int64 FLowestBreakIter _ALIGN_ATTRIBUTE(8);
			__int64 __fastcall GetLowestBreakIter();
			
		protected:
			__fastcall TLoopStateFlag64();
			bool __fastcall ShouldExit(__int64 ThisIter)/* overload */;
			bool __fastcall ShouldExit()/* overload */;
			__property __int64 LowestBreakIter = {read=GetLowestBreakIter};
		public:
			/* TObject.Destroy */ inline __fastcall virtual ~TLoopStateFlag64() { }
			
		};
		
		
		
	protected:
		virtual void __fastcall DoBreak();
		virtual bool __fastcall DoShouldExit();
		virtual System::Variant __fastcall DoGetLowestBreakIteration();
		
	private:
		TLoopStateFlag64* FSharedLoopStateFlags;
		__int64 FCurrentIteration;
		
	public:
		__fastcall TLoopState64(TLoopStateFlag64* ALoopStateFlags);
		__property __int64 CurrentIteration = {read=FCurrentIteration, write=FCurrentIteration};
	public:
		/* TObject.Destroy */ inline __fastcall virtual ~TLoopState64() { }
		
	};
	
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	class DELPHICLASS TReplicableTask;
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	class PASCALIMPLEMENTATION TReplicableTask : public TTask
	{
		typedef TTask inherited;
		
	private:
		int FParallelism;
		
	private:
		// __classmethod void __fastcall Create@();
		
	protected:
		void __fastcall SetParallelism();
		virtual bool __fastcall ShouldCreateReplica();
		virtual TTask* __fastcall CreateReplicaTask(const System::Sysutils::_di_TProc AProc, TTask* AParent, TTask::TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag);
		virtual void __fastcall QueueEvents();
		__fastcall TReplicableTask(System::TObject* Sender, System::Classes::TNotifyEvent Event, const System::Sysutils::_di_TProc AProc, TThreadPool* const APool, TTask* const AParent, int AParallelism, TTask::TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag)/* overload */;
		
	private:
		// __classmethod void __fastcall Destroy@();
	protected:
		/* TTask.Create */ inline __fastcall TReplicableTask(System::TObject* Sender, System::Classes::TNotifyEvent Event, const System::Sysutils::_di_TProc AProc, TThreadPool* const APool, TTask* const AParent, TTask::TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag)/* overload */ : TTask(Sender, Event, AProc, APool, AParent, CreateFlags, AParentControlFlag) { }
		
	public:
		/* TTask.Create */ inline __fastcall TReplicableTask()/* overload */ : TTask() { }
		/* TTask.Destroy */ inline __fastcall virtual ~TReplicableTask() { }
		
	};
	
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	class DELPHICLASS TReplicaTask;
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	class PASCALIMPLEMENTATION TReplicaTask : public TTask
	{
		typedef TTask inherited;
		
	private:
		// __classmethod void __fastcall Create@();
		
	protected:
		__fastcall TReplicaTask(const System::Sysutils::_di_TProc AProc, TThreadPool* APool, TTask* AParent, TTask::TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag);
		
	private:
		// __classmethod void __fastcall Destroy@();
	public:
		/* TTask.Destroy */ inline __fastcall virtual ~TReplicaTask() { }
		
	};
	
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	class DELPHICLASS TJoinTask;
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	class PASCALIMPLEMENTATION TJoinTask : public TParallel::TReplicableTask
	{
		typedef TParallel::TReplicableTask inherited;
		
	private:
#ifndef _WIN64
		typedef void __fastcall (__closure *_dt_System_Threading_1)(System::TObject* Sender);
		System::DynamicArray<_dt_System_Threading_1> FEvents;
		System::DynamicArray<System::Sysutils::_di_TProc> FProcs;
#else /* _WIN64 */
		System::TArray__1<System::Classes::TNotifyEvent> FEvents;
		System::TArray__1<System::Sysutils::_di_TProc> FProcs;
#endif /* _WIN64 */
		
	private:
		// __classmethod void __fastcall Create@();
		
	protected:
		__fastcall TJoinTask(System::TObject* Sender, System::Classes::TNotifyEvent *AEvents, const int AEvents_High, const System::Sysutils::_di_TProc *AProcs, const int AProcs_High, TThreadPool* APool);
		static TParallel::TLoopResult __fastcall JoinWorker(const System::DelphiInterface<System::Sysutils::TProc__1<int> > AProc, int ALowInclusive, int AHighExclusive, TThreadPool* APool);
		
	public:
		static _di_ITask __fastcall Create(System::TObject* Sender, System::Classes::TNotifyEvent *AEvents, const int AEvents_High, TThreadPool* APool)/* overload */;
		static _di_ITask __fastcall Create(const System::Sysutils::_di_TProc *AProcs, const int AProcs_High, TThreadPool* APool)/* overload */;
		
	private:
		// __classmethod void __fastcall Destroy@();
	protected:
		/* TReplicableTask.Create */ inline __fastcall TJoinTask(System::TObject* Sender, System::Classes::TNotifyEvent Event, const System::Sysutils::_di_TProc AProc, TThreadPool* const APool, TTask* const AParent, int AParallelism, TTask::TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag)/* overload */ : TParallel::TReplicableTask(Sender, Event, AProc, APool, AParent, AParallelism, CreateFlags, AParentControlFlag) { }
		
	protected:
		/* TTask.Create */ inline __fastcall TJoinTask(System::TObject* Sender, System::Classes::TNotifyEvent Event, const System::Sysutils::_di_TProc AProc, TThreadPool* const APool, TTask* const AParent, TTask::TCreateFlags CreateFlags, const TThreadPool::_di_IControlFlag AParentControlFlag)/* overload */ : TParallel::TReplicableTask(Sender, Event, AProc, APool, AParent, CreateFlags, AParentControlFlag) { }
		
	public:
		/* TTask.Create */ inline __fastcall TJoinTask()/* overload */ : TParallel::TReplicableTask() { }
		/* TTask.Destroy */ inline __fastcall virtual ~TJoinTask() { }
		
	/* Hoisted overloads: */
		
	public:
		inline _di_ITask __fastcall  Create(System::TObject* Sender, System::Classes::TNotifyEvent Event){ return TTask::Create(Sender, Event); }
		inline _di_ITask __fastcall  Create(const System::Sysutils::_di_TProc Proc){ return TTask::Create(Proc); }
		inline _di_ITask __fastcall  Create(System::TObject* Sender, System::Classes::TNotifyEvent Event, TThreadPool* const APool){ return TTask::Create(Sender, Event, APool); }
		inline _di_ITask __fastcall  Create(const System::Sysutils::_di_TProc Proc, TThreadPool* APool){ return TTask::Create(Proc, APool); }
		
	};
	
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	struct DECLSPEC_DRECORD TStrideManager
	{
	private:
		int FThreshold;
		int FRequest;
		int FCurrentStride;
		int FMaxStride;
		
	public:
		static TParallel::TStrideManager __fastcall NewStrideManager(int AStartStride, int AThreshold);
		int __fastcall NextStride();
	};
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	
#ifndef _WIN64
	#pragma pack(push,8)
#endif /* not _WIN64 */
	struct DECLSPEC_DRECORD TStrideManager64
	{
	private:
		int FThreshold;
		int FRequest;
		_ALIGNAS(8) __int64 FCurrentStride _ALIGN_ATTRIBUTE(8);
		__int64 FMaxStride;
		
	public:
		static TParallel::TStrideManager64 __fastcall NewStrideManager(__int64 AStartStride, __int64 AThreshold);
		__int64 __fastcall NextStride();
	};
#ifndef _WIN64
	#pragma pack(pop)
#endif /* not _WIN64 */
	
	
	
private:
	static TLoopResult __fastcall ForWorker(System::TObject* Sender, TIteratorEvent AEvent, TIteratorStateEvent AStateEvent, const System::DelphiInterface<System::Sysutils::TProc__1<int> > AProc, const System::DelphiInterface<System::Sysutils::TProc__2<int,TLoopState*> > AProcWithState, int ALowInclusive, int AHighExclusive, int AStride, TThreadPool* APool);
	static TLoopResult __fastcall ForWorker64(System::TObject* Sender, TIteratorEvent64 AEvent, TIteratorStateEvent64 AStateEvent, const System::DelphiInterface<System::Sysutils::TProc__1<__int64> > AProc, const System::DelphiInterface<System::Sysutils::TProc__2<__int64,TLoopState*> > AProcWithState, __int64 ALowInclusive, __int64 AHighExclusive, __int64 AStride, TThreadPool* APool);
	
public:
	static TLoopResult __fastcall For(System::TObject* Sender, int ALowInclusive, int AHighInclusive, TIteratorEvent AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int ALowInclusive, int AHighInclusive, TIteratorEvent AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int ALowInclusive, int AHighInclusive, TIteratorStateEvent AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int ALowInclusive, int AHighInclusive, TIteratorStateEvent AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int AStride, int ALowInclusive, int AHighInclusive, TIteratorEvent AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int AStride, int ALowInclusive, int AHighInclusive, TIteratorEvent AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int AStride, int ALowInclusive, int AHighInclusive, TIteratorStateEvent AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, int AStride, int ALowInclusive, int AHighInclusive, TIteratorStateEvent AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<int> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<int> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<int,TLoopState*> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<int,TLoopState*> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(int AStride, int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<int> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(int AStride, int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<int> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(int AStride, int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<int,TLoopState*> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(int AStride, int ALowInclusive, int AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<int,TLoopState*> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorEvent64 AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorEvent64 AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorStateEvent64 AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorStateEvent64 AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorEvent64 AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorEvent64 AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorStateEvent64 AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(System::TObject* Sender, __int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, TIteratorStateEvent64 AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(__int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<__int64> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(__int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<__int64> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(__int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<__int64,TLoopState*> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(__int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<__int64,TLoopState*> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(__int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<__int64> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(__int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__1<__int64> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static TLoopResult __fastcall For(__int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<__int64,TLoopState*> > AIteratorEvent)/* overload */;
	static TLoopResult __fastcall For(__int64 AStride, __int64 ALowInclusive, __int64 AHighInclusive, const System::DelphiInterface<System::Sysutils::TProc__2<__int64,TLoopState*> > AIteratorEvent, TThreadPool* APool)/* overload */;
	static _di_ITask __fastcall Join(System::TObject* Sender, System::Classes::TNotifyEvent *AEvents, const int AEvents_High)/* overload */;
	static _di_ITask __fastcall Join(System::TObject* Sender, System::Classes::TNotifyEvent *AEvents, const int AEvents_High, TThreadPool* APool)/* overload */;
	static _di_ITask __fastcall Join(System::TObject* Sender, System::Classes::TNotifyEvent AEvent1, System::Classes::TNotifyEvent AEvent2)/* overload */;
	static _di_ITask __fastcall Join(System::TObject* Sender, System::Classes::TNotifyEvent AEvent1, System::Classes::TNotifyEvent AEvent2, TThreadPool* APool)/* overload */;
	static _di_ITask __fastcall Join(const System::Sysutils::_di_TProc *AProcs, const int AProcs_High)/* overload */;
	static _di_ITask __fastcall Join(const System::Sysutils::_di_TProc *AProcs, const int AProcs_High, TThreadPool* APool)/* overload */;
	static _di_ITask __fastcall Join(const System::Sysutils::_di_TProc AProc1, const System::Sysutils::_di_TProc AProc2)/* overload */;
	static _di_ITask __fastcall Join(const System::Sysutils::_di_TProc AProc1, const System::Sysutils::_di_TProc AProc2, TThreadPool* APool)/* overload */;
public:
	/* TObject.Create */ inline __fastcall TParallel() : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TParallel() { }
	
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE bool __fastcall GetThreadPoolInteractive(TThreadPool* APool);
extern DELPHI_PACKAGE void __fastcall SetThreadPoolInteractive(TThreadPool* APool, bool AValue);
}	/* namespace Threading */
}	/* namespace System */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_SYSTEM_THREADING)
using namespace System::Threading;
#endif
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_SYSTEM)
using namespace System;
#endif

//-- user supplied -----------------------------------------------------------
#include <SystemThreading.h>

#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// System_ThreadingHPP
