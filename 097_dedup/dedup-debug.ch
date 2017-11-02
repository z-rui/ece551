@x
const std::string& add(const std::string& path)
{
	Slot *p, *pos, *end;
	std::ifstream f(path.c_str());
	size_t h = hash(f);
	p = pos = &hvec[h % hsiz];
	end = &hvec[hsiz];
	while (!p->path.empty()) {
		@<if found a duplicate, return the stored path@>@;
		p++; /* linear probing */
		if (p == end) {
			p = hvec; /* wrap around */
		}
	}
	@<add |path| to hash table@>@;
	return path;
@y
const std::string& add(const std::string& path)
{
	Slot *p, *pos, *end;
	std::ifstream f(path.c_str());
	size_t h = hash(f);
	std::cerr << "hash value = " << h << ", main position = "
		<< (h % hsiz) << std::endl;
	p = pos = &hvec[h % hsiz];
	end = &hvec[hsiz];
	while (!p->path.empty()) {
		@<if found a duplicate, return the stored path@>@;
		p++; /* linear probing */
		if (p == end) {
			p = hvec; /* wrap around */
		}
	}
	std::cerr << "no match in hash table, stopped at "
		<< (p - hvec) << std::endl;
	@<add |path| to hash table@>@;
	return path;
@z

@x
	*q = *p;
@y
	*q = *p;
	std::cerr << "rehash " << p->path << " "
		<< (p-hvec) << " -> " << (q-nvec) << std::endl;
@z

@x
int Dedup::work(const std::string &path)
{
	const std::string &stored_path = fset.add(path);
	if (&stored_path != &path) {
		/* comparing addresses is OK,
		   due to the implementation of |FileSet::add| */
		std::cout << "#Removing " << path
			<< " (duplicate of " << stored_path
			<< ").\n\nrm " << path << "\n\n";
	}
	return 0;
}
@y
int Dedup::work(const std::string &path)
{
	std::cerr << "working on " << path << std::endl;
	const std::string &stored_path = fset.add(path);
	if (&stored_path != &path) {
		/* comparing addresses is OK,
		   due to the implementation of |FileSet::add| */
		std::cout << "#Removing " << path
			<< " (duplicate of " << stored_path
			<< ").\n\nrm " << path << "\n\n";
		std::cerr << "duplicate\n";
	} else {
		std::cerr << "not duplicate\n";
	}
	return 0;
}
@z
