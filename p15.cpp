#include <utility>
#include <string>
#include <iostream>

typedef long long ll;
typedef struct Treap {
	ll prio;
	ll quality;
	ll size;
	Treap * lc;
	Treap * rc;
} Treap;

Treap * merge(Treap * a, Treap * b) {
	if (a == nullptr) return b;
	if (b == nullptr) return a;
	if (a->prio < b->prio) {
		a->size += b->size;
		a->rc = merge(a->rc, b);
		return a;
	} else {
		b->size += a->size;
		b->lc = merge(a, b->lc);
		return b;
	}
}

ll size(Treap* a) {
	if (a == nullptr) return 0;
	return a->size;
}

std::pair<Treap*, Treap*> split_quality(Treap * a, ll q) {
	if (a == nullptr) return std::make_pair(nullptr, nullptr);
	if (a->quality > q) {
		auto [lsl, lsr] = split_quality(a->lc, q);
		a->size -= size(lsl);
		a->lc = lsr;
		return std::make_pair(lsl, a);
	} else {
		auto [rsl, rsr] = split_quality(a->rc, q);
		a->size -= size(rsr);
		a->rc = rsl;
		return std::make_pair(a, rsr);
	}
}

std::pair<Treap*, Treap*> split_size(Treap * a, ll left_size) {
	if (a == nullptr) return std::make_pair(nullptr, nullptr);
	if (left_size == 0) return std::make_pair(nullptr, a);
	if (left_size == a->size) return std::make_pair(a, nullptr);
	if (size(a->lc) >= left_size) {
		auto [lsl, lsr] = split_size(a->lc, left_size);
		a->size -= left_size;
		a->lc = lsr;
		return std::make_pair(lsl, a);
	} else {
		auto [rsl, rsr] = split_size(a->rc, left_size - size(a->lc) - 1);
		a->size = left_size;
		a->rc = rsl;
		return std::make_pair(a, rsr);
	}
}

Treap* treap = nullptr;

int main() {
	std::string s;
	ll seed = 12304987;
	ll ans = 0;
	ll req_i = 0;
	while (std::cin >> s) {
		if (s == "receive") {
			ll t;
			std::cin >> t;
			auto [left, right] = split_quality(treap, t);
			treap = merge(left, merge(new Treap {
				.prio = seed = (seed * 0xdefaced + 1),
				.quality = t,
				.size = 1,
				.lc = nullptr,
				.rc = nullptr,
			}, right));
		} else {
			auto [left, right] = split_size(treap, size(treap) / 2);
			auto [extract, rr] = split_size(right, 1);
			ans += (++req_i) * extract->quality;
			treap = merge(left, rr);
		}
	}
	std::cout << ans << std::endl;
}
