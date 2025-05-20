#pragma once
#include <functional>
#include <vector>
#include <mutex>
#include <shared_mutex>

enum class callback_type : uint8_t
{
	main,
	extra,
	render,
	attack,
	send_pos,
	count
};

struct callback_t
{
	callback_type type;
	std::function<void( )> function;

	callback_t( callback_type type_t, std::function<void( )> fn ) : type( type_t ), function( fn )
	{ }
	~callback_t( ) = default;

	bool operator==( const callback_t& other ) const noexcept;
	explicit operator bool( ) const noexcept;
};

class callback_system
{
private:
	std::vector<callback_t> m_callbacks;
	mutable std::shared_mutex m_mutex;

public:
	void add( callback_t callback );
	void erase_all( callback_type type ) noexcept;
	bool erase( const callback_t& callback ) noexcept;
	void clear( ) noexcept;
	void call( callback_type type ) const;
	bool empty( ) const noexcept;
	size_t count( callback_type type ) const noexcept;
};