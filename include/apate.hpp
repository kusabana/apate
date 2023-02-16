#ifndef APATE_HPP
#define APATE_HPP

#include <sys/mman.h>

namespace apate {
  namespace impl {
    class protect_guard {
    public:
      protect_guard( void *page, size_t size, int flags )
          : page( page )
          , size( size )
          , flags( flags ) {
        if ( mprotect( page, size, flags ) != 0 ) {
          perror( "apate: unable to change page protections" );
        }
      }
      ~protect_guard( ) { mprotect( page, size, PROT_READ ); }

    private:
      void *page;
      size_t size;
      int flags;
    };
  } // namespace impl

  auto get_page( void *method ) -> void * {
    return std::bit_cast< void * >(
        std::bit_cast< uintptr_t >( method ) &
        ~( sysconf( _SC_PAGE_SIZE ) - 1 ) );
  }

  auto hook( void **method, void *func ) -> void * {
    // align method to page
    auto page = get_page( method );

    // ensure page has correct protection flags
    impl::protect_guard guard( page, 4, PROT_READ | PROT_WRITE );

    auto original = *method;
    *method = func;

    return original;
  }

  class declared {
  public:
    declared( void *addr )
        : method( addr ) { };

    template < typename T > auto hook( T func ) -> void {
      // ensure page has correct protection flags
      impl::protect_guard guard(
          get_page( method ), 4, PROT_READ | PROT_WRITE );

      original = *std::bit_cast< void ** >( method );
      *std::bit_cast< void ** >( method ) = std::bit_cast< void * >( func );
    }

    auto unhook( ) -> void {
      impl::protect_guard guard(
          get_page( method ), 4, PROT_READ | PROT_WRITE );
      *std::bit_cast< void ** >( method ) = original;
    }

    template < typename T > auto get_original( ) -> T {
      return std::bit_cast< T >( original );
    }

  private:
    void *original;
    void *method;
  };
} // namespace apate

#endif // APATE_HPP