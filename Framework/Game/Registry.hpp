#pragma once

#include <cassert>
#include <type_traits>

#include "Framework/Game/GameMode.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/Component/EntityByLocalMap.hpp"
#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/SyncManager.hpp"

#include "3rdparty/include/entt/entt.hpp"

// 包装一下entt::registry,方便管理unique和local
class Registry
{
public:
	Registry(GameMode game_mode,Locator&& locator)
		: game_mode_(game_mode)
		, local_map_(registry_.set<EntityByLocalMap>())
		, unique_map_(registry_.set<EntityByUniqueIdMap>())
	{
		registry_.set<Locator>(std::move(locator));

		registry_.on_construct<Local>().connect<&Registry::ConnectLocalIdToEntity>();
		registry_.on_destroy<Local>().connect<&Registry::DisconnectLocalIdToEntity>();

		registry_.on_construct<Remote>().connect<&Registry::ConnectUniqueIdToEntity>();
		registry_.on_destroy<Remote>().connect<&Registry::DisconnectUniqueIdToEntity>();
	}

	[[nodiscard]] bool valid(const entt::registry::entity_type entity) const {
		return registry_.valid(entity);
	}

	[[nodiscard]] entt::registry::entity_type create() {
		return registry_.create();
	}

	entt::registry::version_type destroy(const entt::registry::entity_type entity) {
		return registry_.destroy(entity);
	}

	template<typename It>
	void destroy(It first, It last) {
		registry_.destroy(first, last);
	}

	template<typename Component, typename... Args>
	decltype(auto) emplace(const entt::registry::entity_type entity, Args &&...args) {
		return registry_.emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template<typename Component, typename It>
	void insert(It first, It last, const Component &value = {}) {
		registry_.insert<Component>(first, last, value);
	}

	template<typename Component, typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<std::decay_t<typename std::iterator_traits<CIt>::value_type>, Component>>>
	void insert(EIt first, EIt last, CIt from) {
		registry_.insert<Component>(first, last, from);
	}

	template<typename Component, typename... Args>
	decltype(auto) emplace_or_replace(const entt::registry::entity_type entity, Args &&...args) {
		registry_.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
	}

	template<typename Component, typename... Func>
	decltype(auto) patch(const entt::registry::entity_type entity, Func &&...func) {
		registry_.patch<Component>(entity, std::forward<Func>(func)...);
	}

	template<typename Component, typename... Args>
	decltype(auto) replace(const entt::registry::entity_type entity, Args &&...args) {
		registry_.replace<Component>(entity, std::forward<Args>(args)...);
	}

	template<typename Component, typename... Other>
	entt::registry::size_type remove(const entt::registry::entity_type entity) {
		return registry_.remove<Component>(entity);
	}

	template<typename Component, typename... Other, typename It>
	entt::registry::size_type remove(It first, It last) {
		return registry_.remove<Component>(first, last);
	}

	template<typename... Component>
	[[nodiscard]] bool all_of(const entt::registry::entity_type entity) const {
		return registry_.all_of<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] bool any_of(const entt::registry::entity_type entity) const {
		return registry_.any_of<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] decltype(auto) get([[maybe_unused]] const entt::registry::entity_type entity) const {
		return registry_.get<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] decltype(auto) get([[maybe_unused]] const entt::registry::entity_type entity) {
		return registry_.get<Component...>(entity);
	}

	template<typename Component, typename... Args>
	[[nodiscard]] decltype(auto) get_or_emplace(const entt::registry::entity_type entity, Args &&...args) {
		return registry_.get_or_emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template<typename... Component>
	[[nodiscard]] auto try_get([[maybe_unused]] const entt::registry::entity_type entity) const {
		return registry_.try_get<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] auto try_get([[maybe_unused]] const entt::registry::entity_type entity) {
		return registry_.try_get<Component...>(entity);
	}

	template<typename... Component>
	void clear() {
		registry_.clear<Component...>();
	}

	template<typename Func>
	void each(Func func) const {
		registry_.each(std::forward<Func>(func));
	}

	template<typename Component>
	[[nodiscard]] auto on_construct() {
		return registry_.on_construct<Component>();
	}

	template<typename Component>
	[[nodiscard]] auto on_update() {
		return registry_.on_update<Component>();
	}

	template<typename Component>
	[[nodiscard]] auto on_destroy() {
		return registry_.on_destroy<Component>();
	}

	template<typename Component, typename... Other, typename... Exclude>
	[[nodiscard]] entt::basic_view<entt::entity, entt::get_t<std::add_const_t<Component>, std::add_const_t<Other>...>, entt::exclude_t<Exclude...>> view(entt::exclude_t<Exclude...> = {}) const {
		return registry_.view<Component, Other...>(entt::exclude<Exclude...>);
	}

	template<typename Component, typename... Other, typename... Exclude>
	[[nodiscard]] entt::basic_view<entt::entity, entt::get_t<Component, Other...>, entt::exclude_t<Exclude...>> view(entt::exclude_t<Exclude...> = {}) {
		return registry_.view<Component, Other...>(entt::exclude<Exclude...>);
	}

	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<Owned...>, entt::get_t<Get...>, entt::exclude_t<Exclude...>> group(entt::get_t<Get...>, entt::exclude_t<Exclude...> = {}) {
		return registry_.group<Owned...>(entt::get<Get...>, entt::exclude<Exclude...>);
	}

	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<std::add_const_t<Owned>...>, entt::get_t<std::add_const_t<Get>...>, entt::exclude_t<Exclude...>> group_if_exists(entt::get_t<Get...>, entt::exclude_t<Exclude...> = {}) const {
		return registry_.group<Owned...>(entt::get<Get...>, entt::exclude<Exclude...>);
	}

	template<typename... Owned, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<Owned...>, entt::get_t<>, entt::exclude_t<Exclude...>> group(entt::exclude_t<Exclude...> = {}) {
		return registry_.group<Owned...>(entt::exclude<Exclude...>);
	}

	template<typename... Owned, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<std::add_const_t<Owned>...>, entt::get_t<>, entt::exclude_t<Exclude...>> group_if_exists(entt::exclude_t<Exclude...> = {}) const {
		return registry_.group_if_exists<Owned...>(entt::exclude<Exclude...>);
	}

	template<typename... Component>
	[[nodiscard]] bool sortable() const {
		return registry_.sortable<Component...>();
	}

	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] bool sortable(const entt::basic_group<entt::entity, entt::owned_t<Owned...>, entt::get_t<Get...>, entt::exclude_t<Exclude...>>& group) ENTT_NOEXCEPT {
		return registry_.sortable<Owned...>(group);
	}

	template<typename Component, typename Compare, typename Sort = entt::std_sort, typename... Args>
	void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
		registry_.sort<Component>(std::forward<Compare>(compare), std::forward<Sort>(algo), std::forward<Args>(args)...);
	}

	template<typename To, typename From>
	void sort() {
		registry_.sort<To, From>();
	}

	template<typename Type, typename... Args>
	Type &set(Args &&...args) {
		return registry_.set<Type>(std::forward<Args>(args)...);
	}

	template<typename Type>
	void unset() {
		return registry_.unset<Type>();
	}

	template<typename Type, typename... Args>
	[[nodiscard]] Type &ctx_or_set(Args &&...args) {
		return registry_.ctx_or_set<Type>(std::forward<Args>(args)...);
	}

	template<typename Type>
	[[nodiscard]] std::add_const_t<Type> *try_ctx() const {
		return registry_.try_ctx<Type>();
	}

	template<typename Type>
	[[nodiscard]] Type *try_ctx() {
		return registry_.try_ctx<Type>();
	}

	template<typename Type>
	[[nodiscard]] std::add_const_t<Type> &ctx() const {
		return registry_.ctx<Type>();
	}

	/*! @copydoc ctx */
	template<typename Type>
	[[nodiscard]] Type &ctx() {
		return registry_.ctx<Type>();
	}

	template<typename Func>
	void ctx(Func func) const {
		registry_.ctx(std::forward<Func>(func));
	}

public:
	/*const entt::registry& registry() const
	{
		return registry_;
	}

	entt::registry& registry()
	{
		return registry_;
	}*/

	operator entt::registry&()
	{
		return registry_;
	}

	operator const entt::registry& () const
	{
		return registry_;
	}

	entt::entity create_predict(uint32_t player)
	{
		auto e = registry_.create();
		if (game_mode_ == GameMode::kAuthority)
		{
			registry_.emplace<Remote>(e, e);
		}

		auto& sync_manager = registry_.ctx<SyncManager>();
		auto& sync = sync_manager.sync_instances[player];
		registry_.emplace<Local>(e, sync.owner, sync.identifier++);

		return e;
	}

	// 根据source预测创建可以在客户端之间同步的实体
	entt::entity create_predict(entt::entity source)
	{
		if (auto player = registry_.try_get<Player>(source))
		{
			// 客户端控制的实体
			return create_predict(player->id);
		}
		else
		{
			return create_predict(ServerInstance);
		}
	}

	entt::entity create_predict()
	{
		assert(game_mode_ == GameMode::kAuthority && "只能在服务端上执行");

		return create_predict(ServerInstance);
	}

	// 创建在客户端之间同步的实体
	entt::entity create_unique() 
	{
		auto e = registry_.create();
		registry_.emplace<Remote>(e, e);

		return e;
	}

	// 根据source预测创建本地实体,不在客户端之间同步
	entt::entity create_local(entt::entity source)
	{
		if (auto player = registry_.try_get<Player>(source))
		{
			// 客户端控制的实体
			auto e = registry_.create();
			auto& sync_manager = registry_.ctx<SyncManager>();
			auto& sync = sync_manager.sync_instances[player->id];
			registry_.emplace<Local>(e, sync.owner, sync.identifier++);

			return e;
		}
		
		return create_local();
	}

	// 创建服务端本地实体
	entt::entity create_local()
	{
		//服务端控制的实体
		auto e = registry_.create();
		auto& sync_manager = registry_.ctx<SyncManager>();
		auto& sync = sync_manager.sync_instances[ServerInstance];
		registry_.emplace<Local>(e, sync.owner, sync.identifier++);

		return e;
	}

	bool valid(Remote id) const
	{
		return get(id) != entt::null;
	}

	bool valid(const Local& id) const 
	{
		return get(id) != entt::null;
	}

	entt::entity get(Remote id) const
	{
		auto it = unique_map_.value.find(id);
		if (it == unique_map_.value.end())
		{
			return entt::null;
		}

		entt::entity e = it->second;

		assert(registry_.valid(e));

		return e;
	}

	entt::entity get(const Local& local) const
	{
		auto it = local_map_.value.find(local);
		if (it == local_map_.value.end())
		{
			return entt::null;
		}

		entt::entity e = it->second;

		assert(registry_.valid(e));

		return e;
	}
private:
	static void ConnectUniqueIdToEntity(entt::registry& registry, entt::entity e)
	{
		auto& unique_id = registry.get<Remote>(e);

		auto& map = registry.ctx<EntityByUniqueIdMap>();
		map.value.emplace(unique_id, e);
	}

	static void DisconnectUniqueIdToEntity(entt::registry& registry, entt::entity e)
	{
		auto& unique_id = registry.get<Remote>(e);

		auto& map = registry.ctx<EntityByUniqueIdMap>();
		map.value.erase(unique_id);
	}

	static void ConnectLocalIdToEntity(entt::registry& registry, entt::entity e)
	{
		auto& local_id = registry.get<Local>(e);

		auto& map = registry.ctx<EntityByLocalMap>();
		map.value.emplace(local_id, e);
	}

	static void DisconnectLocalIdToEntity(entt::registry& registry, entt::entity e)
	{
		auto& local_id = registry.get<Local>(e);

		auto& map = registry.ctx<EntityByLocalMap>();
		map.value.erase(local_id);
	}

	entt::registry registry_;
	GameMode game_mode_;
	EntityByLocalMap& local_map_;
	EntityByUniqueIdMap& unique_map_;
};