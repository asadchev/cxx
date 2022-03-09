// ;; This buffer is for text that is not saved, and for Lisp evaluation.
// ;; To create a file, visit it with C-x C-f and enter text in its buffer.

#include <array>
#include <tuple>
#include <iostream>

template<size_t N>
constexpr auto cart_prod_index(size_t i, std::array<size_t,N> dims) {
  std::array<size_t,N> idx = { 1, 3, 4};
  for (size_t j = 0; j < N; ++j) {
    size_t nj = dims[j];
    idx[j] = i%nj;
    i /= nj;
  }
  return idx;
}

template<size_t K, size_t ... Dims, class F, size_t ... Is>
auto cart_prod_apply(F f, std::index_sequence<Is...>) {
  static_assert(sizeof...(Dims) == sizeof...(Is));
  constexpr auto N = (Dims * ...);
  if constexpr (N == K) { return; }
  else {
    constexpr auto index = cart_prod_index(K, std::array{ Dims... });
    f(index);
    std::tuple(
      std::integral_constant<size_t,std::get<Is>(index)>{}...
    );
    auto index_constant = [=](auto && idx) {
      return std::integral_constant<size_t,std::get<idx>(index)>{};
    };
    std::tuple(
      index_constant(std::integral_constant<size_t,Is>{})...
    );
    std::tuple(
      std::get<index[Is]>(
        std::tuple<int,double,float>{}
      )...
    );
    cart_prod_apply<K+1,Dims...>(f, std::index_sequence<Is...>{});
  }
}

int main() {
  cart_prod_apply<0,3,1,2>(
    [](auto args) {
      std::apply(
        [](auto ... args) {
          std::cout << "[";
          ((std::cout << " " << args), ...);
          std::cout << " ]" << std::endl;
        }
        ,args
      );
    },
    std::make_index_sequence<3>{}
  );
}
