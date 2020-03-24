# Visibility Control
Put simply, it hides most of the ELF symbols which would have previously (and unnecessarily) been public. This means:
 - It very substantially improves load times of your DSO (Dynamic Shared Object). For example, a huge C++ template-based library which was tested (the TnFOX Boost.Python bindings library) now loads in eight seconds rather than over six minutes! 
 - It lets the optimiser produce better code. PLT indirections (when a function call or variable access must be looked up via the Global Offset Table such as in PIC code) can be completely avoided, thus substantially avoiding pipeline stalls on modern processors and thus much faster code. Furthermore when most of the symbols are bound locally, they can be safely elided (removed) completely through the entire DSO. This gives greater latitude especially to the inliner which no longer needs to keep an entry point around "just in case". 
 - It reduces the size of your DSO by 5-20%. ELF's exported symbol table format is quite a space hog, giving the complete mangled symbol name which with heavy template usage can average around 1000 bytes. C++ templates spew out a huge amount of symbols and a typical C++ library can easily surpass 30,000 symbols which is around 5-6Mb! Therefore if you cut out the 60-80% of unnecessary symbols, your DSO can be megabytes smaller! 
 - Much lower chance of symbol collision. The old woe of two libraries internally using the same symbol for different things is finally behind us with this patch. Hallelujah! 

<p>Some examples of the syntax:</p>

<pre><code>
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

extern "C" DLL_PUBLIC void function(int a);
class DLL_PUBLIC SomeClass
{
   int c;
   DLL_LOCAL void privateMethod();  // Only for use within this DSO
public:
   Person(int _c) : c(_c) { }
   static void foo(int a);
};
</code></pre>

# Step-by-step guide
The following instructions are how to add full support to your library, yielding the highest quality code with the greatest reductions in binary size, load times and link times. All new code should have this support from the beginning! And it's worth your while especially in speed critical libraries to spend the few days required to implement it fully - it's a once off investment of time with nothing but good resulting forever more. You can however add basic support to your library in far less time though it is not recommended that you do so.

- Place something along the lines of the following code in your master header file (or a specific header that you will include everywhere). This code is taken from the aforementioned TnFOX library: 

<pre><code>
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define FOX_HELPER_DLL_IMPORT __declspec(dllimport)
  #define FOX_HELPER_DLL_EXPORT __declspec(dllexport)
  #define FOX_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define FOX_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define FOX_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define FOX_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define FOX_HELPER_DLL_IMPORT
    #define FOX_HELPER_DLL_EXPORT
    #define FOX_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define FOX_API and FOX_LOCAL.
// FOX_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// FOX_LOCAL is used for non-api symbols.

#ifdef FOX_DLL // defined if FOX is compiled as a DLL
  #ifdef FOX_DLL_EXPORTS // defined if we are building the FOX DLL (instead of using it)
    #define FOX_API FOX_HELPER_DLL_EXPORT
  #else
    #define FOX_API FOX_HELPER_DLL_IMPORT
  #endif // FOX_DLL_EXPORTS
  #define FOX_LOCAL FOX_HELPER_DLL_LOCAL
#else // FOX_DLL is not defined: this means FOX is a static lib.
  #define FX_API
  #define FOX_LOCAL
#endif // FOX_DLL
</code></pre>