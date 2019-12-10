#include "jep_utils.h"

// TODO: implement multibyte number I/O in a way that
// is independent of endianness


/*--------------------------------------------*/
/*                 metadata                   */
/*--------------------------------------------*/
static const jep_byte dict_begin = 0x01;
static const jep_byte dict_byte = 0x02;
static const jep_byte dict_code = 0x03;
static const jep_byte dict_end = 0x04;
static const jep_byte data_begin = 0x05;
static const jep_byte data_end = 0x06;




/*--------------------------------------------*/
/*      resource allocation and disposal      */
/*--------------------------------------------*/

/**
 * Creates a Huffman leaf node.
 *
 * Returns:
 *   huff_node - a new leaf node
 */
static jep_huff_node* create_leaf_node();

/**
 * Frees the resources allocated for a Huffman leaf node.
 *
 * Params:
 *   huff_node - a Huffman leaf node
 */
static void destroy_leaf_node(jep_huff_node* node);

/**
 * Creates a dictionary of bitcodes.
 *
 * Params:
 *   size_t - the number of elements in the dictionary
 *
 * Returns:
 *   huff_dict - a new bitcode dictionary
 */
static jep_huff_dict* create_dict(size_t count);

/**
 * Frees the resources allocated for a bitcode dictionary.
 *
 * Params:
 *   huff_dict - a bitcode dictionary
 */
static void destroy_dict(jep_huff_dict* dict);

/**
 * Creates an empty, unsorted Huffman tree.
 *
 * Returns:
 *   huff_node - a new, unorganized Huffman tree
 */
static jep_huff_tree* create_tree();

/**
 * Frees the resources allocated for a Huffman tree.
 *
 * Params:
 *   huff_node - a Huffman tree
 */
static void destroy_tree(jep_huff_tree* tree);

/**
 * Creates a new Huffman Coding context.
 *
 * Returns:
 *   huff_code - a new Huffman Coding context.
 */
static jep_huff_code* create_huff_code();




/*--------------------------------------------*/
/*            tree construction               */
/*--------------------------------------------*/

/**
 * Adds a leaf node to a Huffman tree.
 *
 * Params:
 *   huff_tree - a Huffman tree
 *   huff_node - a Huffman leaf node
 */
static void add_leaf_node(jep_huff_tree* tree, jep_huff_node* node);

/**
 * Sorts the leaf nodes of a Huffman tree in order of increasing frequency.
 *
 * Params:
 *   huff_tree - an unsorted Huffman tree
 */
static void sort_leaf_nodes(jep_huff_tree* tree);

/**
 * Assembles a list of Huffman leaf nodes into a Huffman tree.
 * The leaf nodes should already be sorted by frequency.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   huff_tree - an unconstructed Huffman tree
 *
 * Returns:
 *   int - 1 on success or 0 on failure.
 */
static int construct_tree(jep_huff_tree* tree);

/**
 * Builds a Huffman tree based on a dictionary of bitcodes.
 *
 * Params:
 *   huff_dict - a bitcode dictionary
 *
 * Returns:
 *   huff_tree - a Huffman tree
 */
static jep_huff_tree* reconstruct_tree(jep_huff_dict* dict);

/**
 * Assigns bitcodes to the leaves of a Huffman tree.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   huff_tree - a Huffman tree
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
static int assign_bitcodes(jep_huff_tree* tree);

/**
 * Recursively assigns a bitcode to a Huffman leaf node and all of its
 * leaf nodes.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   huff_node - the root node of a Huffman tree
 *   unsigned long - the current level within the tree
 *   jep_bitstring - a bitstring populated based on the current branch and level
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
static int assign_bitcode(
	jep_huff_node* node,
	unsigned long level,
	jep_bitstring* bs
);

/**
 * Constructs a branch of a Huffman tree.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   huff_node - a Huffman node
 *   jep_bitstring - a bitstring containing the bitcode for the current branch
 *   huff_sym - the raw data to be stored at the current node
 *   jep_byte - which byte in the bitstring is currently being used
 *   unsigned long - which bit in the current byte is currently being used
 *   unsigned long - the number of branches traversed
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
static int build_branch(
	jep_huff_node** node,
	jep_bitstring* bs,
	jep_huff_sym data,
	jep_byte byte,
	unsigned long bit,
	unsigned long bit_count
);




/*--------------------------------------------*/
/*                buffer I/O                  */
/*--------------------------------------------*/

/**
 * Reads a bitcode dictionary from a byte buffer.
 *
 * Params:
 *   jep_byte_buffer - a collection of bytes encoded with Huffman Coding
 *   size_t - the position in the byte buffer to start reading
 *
 * Returns:
 *   huff_dict - a bitcode dictionary
 */
static jep_huff_dict* read_dict_from_buffer(jep_byte_buffer* data, size_t* pos);

/**
 * Reads a bitcode dictionary from a byte buffer.
 *
 * Params:
 *   jep_byte_buffer - a collection of bytes encoded with Huffman Coding
 *   size_t - the position in the byte buffer to start reading
 *
 * Returns:
 *   jep_bitstring - a bitstring containing encoded data
 */
static jep_bitstring* read_data_from_buffer(jep_byte_buffer* data, size_t* pos);

/**
 * Writes a bticode dictionary to a byte buffer.
 *
 * Params:
 *   huff_dict - a bitcode dictionary to write
 *   jep_byte_buffer - a byte buffer to receive the data
 */
static void write_dict_to_buffer(jep_huff_dict* dict, jep_byte_buffer* bb);

/**
 * Writes a bitstring to a byte buffer.
 *
 * Params:
 *   jep_bitstring - a bitstring to write
 *   jep_byte_buffer - a byte buffer to receive the data
 */
static void write_data_to_buffer(jep_bitstring* data, jep_byte_buffer* bb);

/**
 * Reads bytes from a buffer.
 *
 * Params:
 *   jep_byte - a reference to memory to receive the bytes
 *   jep_byte_buffer - a buffer of bytes
 *   size_t - the number of bytes to read
 *   size_t - a reference to the position from which to start reading
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
static int read_bytes_from_buffer(jep_byte* b, jep_byte_buffer* bb, size_t n, size_t* pos);

/**
 * Writes bytes to a buffer.
 *
 * Params:
 *   jep_byte - a reference to memory to supply the bytes
 *   jep_byte_buffer - a buffer of bytes
 *   size_t - the number of bytes to write
 *   size_t - a reference to the position from which to start writing
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
static int write_bytes_to_buffer(const jep_byte* b, jep_byte_buffer* bb, size_t n, size_t* pos);




/*--------------------------------------------*/
/*                public API                  */
/*--------------------------------------------*/

JEP_UTILS_API jep_huff_code* JEP_UTILS_CALL
jep_huff_encode(jep_byte_buffer* raw)
{
	jep_huff_code* hc;
	jep_huff_tree* tree;
	jep_huff_dict* dict;
	jep_bitstring* data;

	jep_huff_sym bytes[UCHAR_MAX + 1];
	unsigned long unique;
	unsigned long i;
	unsigned long j;

	if (raw == NULL)
		return NULL;

	hc = create_huff_code();
	data = jep_create_bitstring();
	tree = create_tree();

	/* check for failure to create any of the components */
	if (hc == NULL || data == NULL || tree == NULL)
	{
		jep_destroy_huff_code(hc);
		jep_destroy_bitstring(data);
		destroy_tree(tree);
		return NULL;
	}

	/* prepare dictionary */
	for (i = 0; i < UCHAR_MAX + 1; i++)
	{
		bytes[i].b = (jep_byte)i;
		bytes[i].f = 0;
		bytes[i].w = 0;
		bytes[i].n = 1;
		bytes[i].code = NULL;
	}

	/* determine the unique bytes.
	   any bitstrings created here will be destroyed
	   when the dictionary is destroyed. */
	unique = 0;
	for (i = 0; i < raw->size; i++)
	{
		if (bytes[raw->buffer[i]].f == 0)
		{
			unique++;
			bytes[raw->buffer[i]].code = jep_create_bitstring();

			/* check for failure to create bitstring */
			if (bytes[raw->buffer[i]].code == NULL)
			{
				jep_destroy_huff_code(hc);
				jep_destroy_bitstring(data);
				destroy_tree(tree);
				return NULL;
			}
		}
		bytes[raw->buffer[i]].f++;
	}

	/* create Huffman leaf nodes.
	   any nodes created here will be destroyed
	   when the tree is destroyed. */
	for (i = 0; i < UCHAR_MAX + 1; i++)
	{
		if (bytes[i].f > 0)
		{
			jep_huff_node* node = create_leaf_node();

			/* check for failure to create leaf node */
			if (node == NULL)
			{
				jep_destroy_huff_code(hc);
				jep_destroy_bitstring(data);
				destroy_tree(tree);
				return NULL;
			}

			node->next = NULL;
			node->leaf_1 = NULL;
			node->leaf_2 = NULL;
			node->sym = bytes[i];
			add_leaf_node(tree, node);
		}
	}

	/* build the Huffman tree */
	sort_leaf_nodes(tree);

	/* construct tree and check for failure */
	if (!construct_tree(tree))
	{
		jep_destroy_huff_code(hc);
		jep_destroy_bitstring(data);
		destroy_tree(tree);
		return NULL;
	}

	/* assign bitcodes and check for failure */
	if (!assign_bitcodes(tree))
	{
		jep_destroy_huff_code(hc);
		jep_destroy_bitstring(data);
		destroy_tree(tree);
		return NULL;
	}

	/* create the bitcode dictionary */
	dict = create_dict(unique);

	/* check for failure to create dictionary */
	if (dict == NULL)
	{
		jep_destroy_huff_code(hc);
		jep_destroy_bitstring(data);
		destroy_tree(tree);
		return NULL;
	}

	/* populate the bitcode dictionary */
	for (i = 0, j = 0; i < UCHAR_MAX + 1; i++)
	{
		if (bytes[i].f > 0)
		{
			dict->symbols[j].b = bytes[i].b;
			dict->symbols[j].f = bytes[i].f;
			dict->symbols[j].n = bytes[i].n;
			dict->symbols[j++].code = bytes[i].code;
		}
	}
	dict->count = j;

	/* encode the data */
	for (i = 0; i < raw->size; i++)
	{
		for (j = 0; j < unique; j++)
		{
			if (dict->symbols[j].b == raw->buffer[i])
			{
				/* add bits and check for failure */
				if (!jep_add_bits(data, dict->symbols[j].code))
				{
					jep_destroy_huff_code(hc);
					jep_destroy_bitstring(data);
					destroy_tree(tree);
					return NULL;
				}
			}
		}
	}

	hc->tree = tree;
	hc->dict = dict;
	hc->data = data;

	return hc;
}

JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_huff_decode(jep_huff_code* hc)
{
	jep_byte_buffer* raw;

	jep_huff_node* root;
	jep_huff_node* leaf;
	unsigned long byte = 0;
	unsigned long bit = 0;
	unsigned long i;

	if (hc == NULL)
		return NULL;

	if (hc->tree == NULL || hc->data == NULL)
		return NULL;

	raw = jep_create_byte_buffer();

	if (raw == NULL)
		return NULL;

	root = hc->tree->nodes;
	leaf = root;

	/* traverse the Huffman tree.
	   starting from the root node,
	   if the current bit in the bitstring is 1, we move on
	   to leaf 1 of the current node, otherwise we move to leaf 2.
	   upon reaching the last node of a branch, we add the byte value
	   stored in that node to the output buffer.
	   */
	for (i = 0; i <= hc->data->bit_count; i++)
	{
		/* update the bitstring position variables
		   checking the current bit advances the bit counter.
		   once that has reached CHAR_BIT, we reset it and
		   advance the byte counter. */
		if (bit == CHAR_BIT)
		{
			byte++;
			bit = 0;
		}

		if (leaf->sym.w == 0)
		{
			/* attempt to add the byte to the output
			   buffer and check for failure. */
			if (!jep_append_byte(raw, leaf->sym.b))
			{
				jep_destroy_byte_buffer(raw);
				return NULL;
			}

			leaf = root;
			i--;
		}
		else
		{
			/* determine which leaf node to visit next
			   and advance the bit counter */
			if (hc->data->bytes[byte] & (1 << bit++))
			{
				leaf = leaf->leaf_1;
			}
			else
			{
				leaf = leaf->leaf_2;
			}
		}
	}

	return raw;
}

JEP_UTILS_API jep_huff_code* JEP_UTILS_CALL
jep_read_huff_code_from_buffer(jep_byte_buffer* raw)
{
	jep_huff_tree* tree;
	jep_huff_dict* dict;
	jep_bitstring* bs;
	jep_huff_code* hc;

	size_t pos = 0;

	/* read the dictionary */
	dict = read_dict_from_buffer(raw, &pos);

	if (dict == NULL)
		return NULL;

	/* read the data */
	bs = read_data_from_buffer(raw, &pos);

	if (bs == NULL)
	{
		destroy_dict(dict);
		return NULL;
	}

	/* reconstruct a Huffman tree */
	tree = reconstruct_tree(dict);

	if (tree == NULL)
	{
		destroy_dict(dict);
		jep_destroy_bitstring(bs);
		return NULL;
	}

	/* create and populate the Huffman Coding context */
	hc = create_huff_code();

	if (hc == NULL)
	{
		destroy_dict(dict);
		jep_destroy_bitstring(bs);
		destroy_tree(tree);
		return NULL;
	}

	hc->tree = tree;
	hc->dict = dict;
	hc->data = bs;

	return hc;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_write_huff_code_to_buffer(jep_huff_code* hc, jep_byte_buffer* buffer)
{
	int res = 0;

	if (hc == NULL || buffer == NULL)
		return res;

	if (hc->dict == NULL || hc->data == NULL)
		return res;

	write_dict_to_buffer(hc->dict, buffer);
	write_data_to_buffer(hc->data, buffer);

	res = 1;

	return res;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_huff_code(jep_huff_code* hc)
{
	if (hc == NULL)
		return;

	destroy_dict(hc->dict);
	destroy_tree(hc->tree);
	jep_destroy_bitstring(hc->data);

	free(hc);
}




/*--------------------------------------------*/
/*              implementation                */
/*--------------------------------------------*/

static jep_huff_node* create_leaf_node()
{
	size_t size;
	jep_huff_node* node;

	size = sizeof(jep_huff_node);
	node = (jep_huff_node*)malloc(size);

	if (node == NULL)
		return NULL;

	node->leaf_1 = NULL;
	node->leaf_2 = NULL;
	node->next = NULL;
	node->prev = NULL;
	node->sym.code = NULL;
	node->sym.b = 0;
	node->sym.f = 0;
	node->sym.w = 0;
	node->sym.n = 0;

	return node;
}

static void destroy_leaf_node(jep_huff_node* node)
{
	if (node == NULL)
		return;

	if (node->leaf_1 != NULL)
		destroy_leaf_node(node->leaf_1);

	if (node->leaf_2 != NULL)
		destroy_leaf_node(node->leaf_2);

	/* disposal of bitstrings is handled in destroy_dict */

	free(node);
}

static jep_huff_dict* create_dict(size_t count)
{
	size_t size;
	jep_huff_dict* dict;
	jep_huff_sym* symbols;

	size = sizeof(jep_huff_dict);
	dict = (jep_huff_dict*)malloc(size);

	if (dict == NULL)
		return NULL;

	dict->count = count;

	symbols = (jep_huff_sym*)malloc(sizeof(jep_huff_sym) * count);

	if (symbols == NULL)
	{
		free(dict);
		return NULL;
	}

	dict->symbols = symbols;

	return dict;
}

static void destroy_dict(jep_huff_dict* dict)
{
	if (dict == NULL)
		return;

	unsigned long i;

	for (i = 0; i < dict->count; i++)
		jep_destroy_bitstring(dict->symbols[i].code);

	free(dict->symbols);
	free(dict);
}

static jep_huff_tree* create_tree()
{
	size_t size;
	jep_huff_tree* tree;

	size = sizeof(jep_huff_tree);
	tree = (jep_huff_tree*)malloc(size);

	if (tree == NULL)
		return NULL;

	tree->count = 0;
	tree->nodes = NULL;

	return tree;
}

static void destroy_tree(jep_huff_tree* tree)
{
	if (tree == NULL)
		return;

	jep_huff_node* node = tree->nodes;
	jep_huff_node* next = NULL;

	while (node != NULL)
	{
		next = node->next;
		destroy_leaf_node(node);
		node = next;
	}

	free(tree);
}

static jep_huff_code* create_huff_code()
{
	jep_huff_code* hc;
	jep_bitstring* data;

	hc = (jep_huff_code*)malloc(sizeof(jep_huff_code));

	if (hc == NULL)
		return NULL;

	hc->tree = NULL;
	hc->dict = NULL;

	data = jep_create_bitstring();

	if (data == NULL)
	{
		free(hc);
		return NULL;
	}

	hc->data = data;

	return hc;
}

static void add_leaf_node(jep_huff_tree* tree, jep_huff_node* node)
{
	if (tree->count == 0 && tree->nodes == NULL)
	{
		tree->nodes = node;
		tree->nodes->next = NULL;
		tree->nodes->prev = NULL;
		tree->tail = node;
	}
	else if (tree->nodes != NULL)
	{
		tree->tail->next = node;
		tree->tail->next->prev = tree->tail;
		tree->tail = node;
	}

	tree->count++;
}

/* TODO: add some explanation */
static void sort_leaf_nodes(jep_huff_tree* tree)
{
	if (tree == NULL || tree->nodes == NULL)
		return;

	int sorted = 0;

	while (!sorted)
	{
		sorted = 1;
		jep_huff_node** node = &(tree->nodes);
		while (*node != NULL)
		{
			if ((*node)->next != NULL && (*node)->next->sym.f < (*node)->sym.f)
			{
				sorted = 0;
				jep_huff_node* next = (*node)->next;

				(*node)->next = next->next;

				next->prev = (*node)->prev;

				if (next->next != NULL)
				{
					next->next->prev = *node;
				}

				(*node)->prev = next;

				next->next = *node;

				*node = next;
			}
			else
			{
				node = &((*node)->next);
			}
		}
	}
}

static int construct_tree(jep_huff_tree* tree)
{
	if (tree == NULL || tree->nodes == NULL)
		return 0;

	jep_huff_node** node = &(tree->nodes);

	while (*node != NULL && (*node)->next != NULL)
	{
		jep_huff_node* sum = (jep_huff_node*)malloc(sizeof(jep_huff_node));

		if (sum == NULL)
			return 0;

		sum->next = NULL;
		sum->prev = NULL;

		/* Visual Studio 2019 erroneously
		   warns of possible NULL dereference */
		sum->sym.f = (*node)->sym.f + (*node)->next->sym.f;

		sum->sym.b = 0;
		sum->sym.w = 1;
		sum->sym.n = 1;
		sum->sym.code = NULL;
		sum->leaf_1 = *node;
		sum->leaf_2 = (*node)->next;

		jep_huff_node* next = (*node)->next->next;
		jep_huff_node* last_next = next;
		jep_huff_node* new_head = next;

		while (next != NULL && next->sym.f < sum->sym.f)
		{
			last_next = next;
			next = next->next;
		}

		sum->next = next;

		if (next != NULL)
		{
			sum->prev = next->prev;
			if (next->prev != NULL)
			{
				next->prev->next = sum;
			}
			next->prev = sum;
		}
		else if (next == NULL && last_next != NULL)
		{
			last_next->next = sum;
			sum->prev = last_next;
		}

		if (new_head == NULL || new_head == next)
		{
			*node = sum;
		}
		else
		{
			new_head->prev = NULL;
			*node = new_head;
		}

		tree->count--;
	}

	return 1;
}

static jep_huff_tree* reconstruct_tree(jep_huff_dict* dict)
{
	unsigned long i;
	jep_huff_tree* tree = create_tree();

	if (tree == NULL)
		return NULL;

	jep_huff_node* root = create_leaf_node();

	if (root == NULL)
	{
		destroy_tree(tree);
		return NULL;
	}

	root->sym.w = 1;

	add_leaf_node(tree, root);

	/*
	 * leaf_1 => 1
	 * leaf_2 => 0
	 */

	for (i = 0; i < dict->count; i++)
	{
		jep_bitstring* bs = dict->symbols[i].code;

		int res = build_branch(&root, bs,
			dict->symbols[i],
			0, /* current byte             */
			0, /* position in current byte */
			0  /* current bit count        */
		);

		if (!res)
		{
			destroy_tree(tree);
			return NULL;
		}
	}

	return tree;
}

static int assign_bitcodes(jep_huff_tree* tree)
{
	if (tree == NULL)
		return 0;

	jep_huff_node* node = tree->nodes;

	node->sym.n = 0;

	jep_bitstring* bs = jep_create_bitstring();

	if (node->sym.w == 0)
	{
		if (!jep_add_bit(bs, 0))
		{
			jep_destroy_bitstring(bs);
			return 0;
		}
	}

	int result = assign_bitcode(node, 0, bs);

	jep_destroy_bitstring(bs);

	return result;
}

static void write_dict_to_buffer(jep_huff_dict* dict, jep_byte_buffer* bb)
{
	unsigned long i;
	unsigned long j;

	size_t pos = 0;

	write_bytes_to_buffer(&dict_begin, bb, sizeof(jep_byte), &pos);

	for (i = 0; i < dict->count; i++)
	{
		/* write the byte */
		write_bytes_to_buffer(&dict_byte, bb, sizeof(jep_byte), &pos);
		write_bytes_to_buffer(&(dict->symbols[i].b), bb, sizeof(jep_byte), &pos);

		/* write the bit code metadata */
		write_bytes_to_buffer(&dict_code, bb, sizeof(jep_byte), &pos);
		write_bytes_to_buffer((jep_byte*)(&(dict->symbols[i].code->bit_count)), bb, sizeof(unsigned long), &pos);
		write_bytes_to_buffer((jep_byte*)(&(dict->symbols[i].code->byte_count)), bb, sizeof(unsigned long), &pos);
		write_bytes_to_buffer(&(dict->symbols[i].code->current_bits), bb, sizeof(jep_byte), &pos);

		/* write the bit code data */
		for (j = 0; j < dict->symbols[i].code->byte_count; j++)
		{
			write_bytes_to_buffer(&(dict->symbols[i].code->bytes[j]), bb, sizeof(jep_byte), &pos);
		}
	}

	write_bytes_to_buffer(&dict_end, bb, sizeof(jep_byte), &pos);
}

static void write_data_to_buffer(jep_bitstring* data, jep_byte_buffer* bb)
{
	unsigned long i;

	size_t pos = 0;

	write_bytes_to_buffer(&data_begin, bb, sizeof(jep_byte), &pos);

	/* write the bit string metadata */
	write_bytes_to_buffer((jep_byte*)(&(data->bit_count)), bb, sizeof(unsigned long), &pos);
	write_bytes_to_buffer((jep_byte*)(&(data->byte_count)), bb, sizeof(unsigned long), &pos);
	write_bytes_to_buffer(&(data->current_bits), bb, sizeof(jep_byte), &pos);

	/* write the bit code data */
	for (i = 0; i < data->byte_count; i++)
	{
		write_bytes_to_buffer(&(data->bytes[i]), bb, sizeof(jep_byte), &pos);
	}

	write_bytes_to_buffer(&data_end, bb, sizeof(jep_byte), &pos);
}

static jep_huff_dict* read_dict_from_buffer(jep_byte_buffer* data, size_t* pos)
{
	jep_huff_dict* dict;

	size_t cap = 10;
	size_t count = 0;
	int success = 1;
	jep_huff_sym sym;
	unsigned long ul = 0;
	jep_byte b = 0x00;

	dict = create_dict(sizeof(jep_huff_sym) * cap);

	if (dict == NULL)
		return NULL;

	success = read_bytes_from_buffer(&b, data, 1, pos);

	if (!success || b != dict_begin)
	{
		destroy_dict(dict);
		return NULL;
	}

	while (success)
	{
		success = read_bytes_from_buffer(&b, data, 1, pos);

		if (success && b == dict_byte)
		{
			success = read_bytes_from_buffer(&b, data, 1, pos);

			if (success)
				sym.b = b;
		}
		else if (success && b == dict_code)
		{
			sym.code = jep_create_bitstring();

			/* destroy the bytes since we'll recreate them later */
			free(sym.code->bytes);

			success = read_bytes_from_buffer((jep_byte*)&ul, data, sizeof(unsigned long), pos);
			sym.code->bit_count = ul;

			success = read_bytes_from_buffer((jep_byte*)&ul, data, sizeof(unsigned long), pos);
			sym.code->byte_count = ul;

			success = read_bytes_from_buffer(&b, data, 1, pos);
			sym.code->current_bits = b;

			sym.code->bytes = (jep_byte*)
				malloc(sizeof(jep_byte) * sym.code->byte_count);

			success = read_bytes_from_buffer(sym.code->bytes, data,
				sym.code->byte_count, pos);

			/* resize the dictionary if necessary */
			if (count >= cap)
			{
				size_t new_cap = cap + cap / 2;

				jep_huff_sym* symbols = (jep_huff_sym*)realloc(dict->symbols, sizeof(jep_huff_sym) * new_cap);

				if (symbols == NULL)
				{
					destroy_dict(dict);
					return NULL;
				}

				dict->symbols = symbols;
				cap = new_cap;
			}

			/* add the symbol to the dictionary */
			dict->symbols[count++] = sym;
		}
		else if (b == dict_end)
		{
			success = 0;
		}
	}

	/* free excess memory */
	if (count < cap)
	{
		jep_huff_sym* symbols = (jep_huff_sym*)realloc(dict->symbols, sizeof(jep_huff_sym) * count);

		if (symbols == NULL)
		{
			destroy_dict(dict);
			return NULL;
		}

		dict->symbols = symbols;
	}

	dict->count = count;

	return dict;
}

static jep_bitstring* read_data_from_buffer(jep_byte_buffer* data, size_t* pos)
{
	jep_bitstring* bs = jep_create_bitstring();

	/* dispose of this since we create it later */
	free(bs->bytes);

	unsigned long ul = 0;
	jep_byte b = 0x00;
	size_t flag = 1;

	flag = read_bytes_from_buffer(&b, data, 1, pos);

	if (b == data_begin)
	{
		read_bytes_from_buffer((jep_byte*)&ul, data, sizeof(unsigned long), pos);
		bs->bit_count = ul;

		read_bytes_from_buffer((jep_byte*)&ul, data, sizeof(unsigned long), pos);
		bs->byte_count = ul;

		read_bytes_from_buffer(&b, data, sizeof(jep_byte), pos);
		bs->current_bits = b;

		bs->bytes = (jep_byte*)malloc(sizeof(jep_byte) * bs->byte_count);

		flag = read_bytes_from_buffer(bs->bytes, data, bs->byte_count, pos);
	}

	return bs;
}

static int assign_bitcode(
	jep_huff_node* node,
	unsigned long level,
	jep_bitstring* bs
)
{
	if (node == NULL)
		return 0;

	if (node->sym.w < 1)
	{
		node->sym.n = level;
		if (!jep_add_bits(node->sym.code, bs))
			return 0;
	}

	if (node->leaf_1 != NULL)
	{
		jep_add_bit(bs, 1);

		if (!assign_bitcode(node->leaf_1, level + 1, bs))
			return 0;

		if (!jep_pop_bit(bs))
			return 0;
	}

	if (node->leaf_2 != NULL)
	{
		jep_add_bit(bs, 0);

		if (!assign_bitcode(node->leaf_2, level + 1, bs))
			return 0;

		if (!jep_pop_bit(bs))
			return 0;
	}

	return 1;
}

static int build_branch(
	jep_huff_node** node,
	jep_bitstring* bs,
	jep_huff_sym data,
	jep_byte byte,
	unsigned long bit,
	unsigned long bit_count
)
{
	if (bit_count == bs->bit_count)
	{
		if (*node == NULL)
		{
			*node = create_leaf_node();
			if (*node == NULL)
				return 0;
		}

		(*node)->sym = data;
		(*node)->sym.w = 0;

		return 1;
	}

	if (bit == CHAR_BIT)
	{
		byte++;
		bit = 0;
	}

	jep_huff_node** leaf;

	if (bs->bytes[byte] & (1 << bit++))
		leaf = &((*node)->leaf_1);
	else
		leaf = &((*node)->leaf_2);

	if (*leaf == NULL)
	{
		*leaf = create_leaf_node();

		if (*leaf == NULL)
			return 0;

		(*leaf)->sym.w = 1;
	}

	return build_branch(leaf, bs, data, byte, bit, ++bit_count);
}

static int read_bytes_from_buffer(jep_byte* b, jep_byte_buffer* bb, size_t n, size_t* pos)
{
	if (bb == NULL || bb->buffer == NULL)
		return 0;

	if (*pos >= bb->size)
		return 0;

	size_t i, j;
	size_t start = *pos;
	int result = 0;

	for (i = start, j = 0; i < start + n && i < bb->size; i++)
	{
		b[j++] = bb->buffer[i];
		(*pos)++;
		result++;
	}

	return result;
}

static int write_bytes_to_buffer(const jep_byte* b, jep_byte_buffer* bb, size_t n, size_t* pos)
{
	if (bb == NULL || bb->buffer == NULL)
		return 0;

	if (*pos > bb->size)
		return 0;

	size_t i;
	size_t start = *pos;
	int result = 0;

	for (i = 0; i < n; i++)
	{
		jep_append_byte(bb, b[i]);
		(*pos)++;
		result++;
	}

	return result;
}
