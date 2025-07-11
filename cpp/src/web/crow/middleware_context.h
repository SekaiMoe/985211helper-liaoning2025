#pragma once

#include "../crow/utility.h"
#include "../crow/http_request.h"
#include "../crow/http_response.h"

namespace crow
{
    namespace detail
    {


        template<typename... Middlewares>
        struct partial_context : public black_magic::pop_back<Middlewares...>::template rebind<partial_context>, public black_magic::last_element_type<Middlewares...>::type::context
        {
            using parent_context = typename black_magic::pop_back<Middlewares...>::template rebind<::crow::detail::partial_context>;
            template<int N>
            using partial = typename std::conditional<N == sizeof...(Middlewares) - 1, partial_context, typename parent_context::template partial<N>>::type;

            template<typename T>
            typename T::context& get()
            {
                return static_cast<typename T::context&>(*this);
            }
        };



        template<>
        struct partial_context<>
        {
            template<int>
            using partial = partial_context;
        };


        template<typename... Middlewares>
        struct context : private partial_context<Middlewares...>
        //struct context : private Middlewares::context... // simple but less type-safe
        {
            template<typename CallCriteria, int N, typename Context, typename Container>
            friend typename std::enable_if<(N == 0)>::type after_handlers_call_helper(const CallCriteria& cc, Container& middlewares, Context& ctx, request& req, response& res);
            template<typename CallCriteria, int N, typename Context, typename Container>
            friend typename std::enable_if<(N > 0)>::type after_handlers_call_helper(const CallCriteria& cc, Container& middlewares, Context& ctx, request& req, response& res);

            template<typename CallCriteria, int N, typename Context, typename Container>
            friend typename std::enable_if<(N < std::tuple_size<typename std::remove_reference<Container>::type>::value), bool>::type
              middleware_call_helper(const CallCriteria& cc, Container& middlewares, request& req, response& res, Context& ctx);

            template<typename T>
            typename T::context& get()
            {
                return static_cast<typename T::context&>(*this);
            }

            template<int N>
            using partial = typename partial_context<Middlewares...>::template partial<N>;
        };
    } // namespace detail
} // namespace crow
