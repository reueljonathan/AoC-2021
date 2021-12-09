
struct number_collection {
    size_t length;
    int * items;
};

struct number_collection * create_number_collection();
void add_item(struct number_collection * num_collection, int number);
void print_num_collection(struct number_collection * collection);
