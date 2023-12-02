#include "jep_utils.h"

/*-----------------------------------------------------------------*/
/*                            Metadata                             */
/*-----------------------------------------------------------------*/
static const jep_byte dict_begin = 0x01;
static const jep_byte dict_byte = 0x02;
static const jep_byte dict_code = 0x03;
static const jep_byte dict_end = 0x04;
static const jep_byte data_begin = 0x05;
static const jep_byte data_end = 0x06;

/*-----------------------------------------------------------------*/
/*                              Macros                             */
/*-----------------------------------------------------------------*/

/**
 * Reallocates an array of Huffman symbols for a dictionary.
 *
 * Params:
 *   s - an existing array of Huffman symbols
 *   c - the new capacity of the array
 */
#define resym(a, n) jep_realloc(a, jep_huff_sym, n)

/*-----------------------------------------------------------------*/
/*                        Memory Management                        */
/*-----------------------------------------------------------------*/

/**
 * Creates a Huffman leaf node.
 *
 * Returns:
 *   huff_node - a new leaf node
 */
static jep_huff_node *create_leaf_node();

/**
 * Frees the resources allocated for a Huffman leaf node.
 *
 * Params:
 *   huff_node - a Huffman leaf node
 */
static void destroy_leaf_node(jep_huff_node *node);

/**
 * Creates a dictionary of bitcodes.
 *
 * Params:
 *   size_t - the number of elements in the dictionary
 *
 * Returns:
 *   huff_dict - a new bitcode dictionary
 */
static jep_huff_dict *create_dict(size_t count);

/**
 * Frees the resources allocated for a bitcode dictionary.
 *
 * Params:
 *   huff_dict - a bitcode dictionary
 */
static void destroy_dict(jep_huff_dict *dict);

/**
 * Creates an empty, unsorted Huffman tree.
 *
 * Returns:
 *   huff_node - a new, unorganized Huffman tree
 */
static jep_huff_tree *create_tree();

/**
 * Frees the resources allocated for a Huffman tree.
 *
 * Params:
 *   huff_node - a Huffman tree
 */
static void destroy_tree(jep_huff_tree *tree);

/**
 * Creates a new Huffman Coding context.
 *
 * Returns:
 *   huff_code - a new Huffman Coding context.
 */
static jep_huff_code *create_huff_code();

/*-----------------------------------------------------------------*/
/*                        Tree Construction                        */
/*-----------------------------------------------------------------*/

/**
 * Adds a leaf node to a Huffman tree.
 *
 * Params:
 *   huff_tree - a Huffman tree
 *   huff_node - a Huffman leaf node
 */
static void add_leaf_node(jep_huff_tree *tree, jep_huff_node *node);

/**
 * Sorts the leaf nodes of a Huffman tree in order of increasing frequency.
 *
 * Params:
 *   huff_tree - an unsorted Huffman tree
 */
static void sort_leaf_nodes(jep_huff_tree *tree);

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
static int construct_tree(jep_huff_tree *tree);

/**
 * Builds a Huffman tree based on a dictionary of bitcodes.
 *
 * Params:
 *   huff_dict - a bitcode dictionary
 *
 * Returns:
 *   huff_tree - a Huffman tree
 */
static jep_huff_tree *reconstruct_tree(jep_huff_dict *dict);

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
static int assign_bitcodes(jep_huff_tree *tree);

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
    jep_huff_node *node,
    uint32_t level,
    jep_bitstring *bs);

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
    jep_huff_node **node,
    jep_bitstring *bs,
    jep_huff_sym data,
    jep_byte byte,
    uint32_t bit,
    uint32_t bit_count);

/*-----------------------------------------------------------------*/
/*                           Buffer I/O                            */
/*-----------------------------------------------------------------*/

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
static jep_huff_dict *read_huff_dict(jep_byte_buffer *data, size_t *pos);

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
static jep_bitstring *read_huff_data(jep_byte_buffer *data, size_t *pos);

/**
 * Writes a bticode dictionary to a byte buffer.
 *
 * Params:
 *   huff_dict - a bitcode dictionary to write
 *   jep_byte_buffer - a byte buffer to receive the data
 */
static void write_huff_dict(jep_huff_dict *dict, jep_byte_buffer *bb);

/**
 * Writes a bitstring to a byte buffer.
 *
 * Params:
 *   jep_bitstring - a bitstring to write
 *   jep_byte_buffer - a byte buffer to receive the data
 */
static void write_huff_data(jep_bitstring *data, jep_byte_buffer *bb);

/**
 * Reads bytes from a buffer.
 * Returns the number of bytes successfully read.
 *
 * Params:
 *   jep_byte - a reference to memory to receive the bytes
 *   jep_byte_buffer - a buffer of bytes
 *   size_t - the number of bytes to read
 *   size_t - a reference to the position from which to start reading
 *
 * Returns:
 *   int - The number of byte successfully read from the buffer
 */
static int read_from_buffer(jep_byte *dest,
                            jep_byte_buffer *src,
                            size_t n,
                            size_t *pos);

/**
 * Writes bytes to a buffer.
 * Returns the number of bytes successfully written to the buffer.
 *
 * Params:
 *   jep_byte - a reference to memory to supply the bytes
 *   jep_byte_buffer - a buffer of bytes
 *   size_t - the number of bytes to write
 *   size_t - a reference to the position from which to start writing
 *
 * Returns:
 *   int - The number of byte successfully written to the buffer
 */
static int write_to_buffer(const jep_byte *src,
                           jep_byte_buffer *dest,
                           size_t n,
                           size_t *pos);

/*-----------------------------------------------------------------*/
/*                   Public API Implementation                     */
/*-----------------------------------------------------------------*/

jep_byte_buffer *jep_huff_encode(jep_byte_buffer *raw)
{
    jep_byte_buffer *encoded;
    jep_huff_code *huff;
    jep_huff_tree *tree;
    jep_huff_dict *dict;
    jep_bitstring *data;

    jep_huff_sym bytes[UCHAR_MAX + 1];
    uint32_t unique;
    uint32_t i;
    uint32_t j;

    if (raw == NULL)
    {
        return NULL;
    }

    huff = create_huff_code();
    data = jep_create_bitstring();
    tree = create_tree();

    if (huff == NULL || data == NULL || tree == NULL)
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    for (i = 0; i < UCHAR_MAX + 1; i++)
    {
        bytes[i].byte = (jep_byte)i;
        bytes[i].freq = 0;
        bytes[i].weight = 0;
        bytes[i].depth = 1;
        bytes[i].code = NULL;
    }

    // Any bitstrings created here will be destroyed
    // when the dictionary is destroyed.
    unique = 0;
    for (i = 0; i < raw->size; i++)
    {
        if (bytes[raw->buffer[i]].freq == 0)
        {
            unique++;
            bytes[raw->buffer[i]].code = jep_create_bitstring();

            if (bytes[raw->buffer[i]].code == NULL)
            {
                jep_destroy_huff_code(huff);
                jep_destroy_bitstring(data);
                destroy_tree(tree);
                return NULL;
            }
        }
        bytes[raw->buffer[i]].freq++;
    }

    // Any nodes created here will be destroyed
    // when the tree is destroyed.
    for (i = 0; i < UCHAR_MAX + 1; i++)
    {
        if (bytes[i].freq > 0)
        {
            jep_huff_node *node = create_leaf_node();
            if (node == NULL)
            {
                jep_destroy_huff_code(huff);
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

    sort_leaf_nodes(tree);

    if (!construct_tree(tree))
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    if (!assign_bitcodes(tree))
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    dict = create_dict(unique);
    if (dict == NULL)
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    for (i = 0, j = 0; i < UCHAR_MAX + 1; i++)
    {
        if (bytes[i].freq > 0)
        {
            dict->symbols[j].byte = bytes[i].byte;
            dict->symbols[j].freq = bytes[i].freq;
            dict->symbols[j].depth = bytes[i].depth;
            dict->symbols[j++].code = bytes[i].code;
        }
    }
    dict->count = j;

    for (i = 0; i < raw->size; i++)
    {
        for (j = 0; j < unique; j++)
        {
            if (dict->symbols[j].byte == raw->buffer[i])
            {
                if (!jep_concat_bits(data, dict->symbols[j].code))
                {
                    jep_destroy_huff_code(huff);
                    jep_destroy_bitstring(data);
                    destroy_tree(tree);
                    return NULL;
                }
            }
        }
    }

    huff->tree = tree;
    huff->dict = dict;
    huff->data = data;

    encoded = jep_create_byte_buffer();

    if (encoded == NULL)
    {
        jep_destroy_huff_code(huff);
        return NULL;
    }

    jep_huff_write(huff, encoded);

    return encoded;
}

jep_byte_buffer *jep_huff_decode(jep_byte_buffer *encoded)
{
    jep_byte_buffer *raw;
    jep_huff_code *hc;
    jep_huff_node *root;
    jep_huff_node *leaf;
    uint32_t byte;
    uint32_t bit;
    uint32_t i;

    if (encoded == NULL)
    {
        return NULL;
    }

    hc = jep_huff_read(encoded);

    if (hc == NULL)
    {
        return NULL;
    }

    raw = jep_create_byte_buffer();

    if (raw == NULL)
    {
        jep_destroy_huff_code(hc);
        return NULL;
    }

    root = hc->tree->nodes;
    leaf = root;
    bit = byte = 0;

    for (i = 0; i <= hc->data->bit_count; i++)
    {
        if (bit == CHAR_BIT)
        {
            byte++;
            bit = 0;
        }

        if (leaf->sym.weight == 0)
        {
            if (!jep_append_byte(raw, leaf->sym.byte))
            {
                jep_destroy_byte_buffer(raw);
                return NULL;
            }

            leaf = root;
            i--;
        }
        else
        {
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

jep_huff_code *jep_huff_read(jep_byte_buffer *raw)
{
    jep_huff_tree *tree;
    jep_huff_dict *dict;
    jep_bitstring *bs;
    jep_huff_code *hc;

    size_t pos = 0;

    dict = read_huff_dict(raw, &pos);

    if (dict == NULL)
    {
        return NULL;
    }

    bs = read_huff_data(raw, &pos);

    if (bs == NULL)
    {
        destroy_dict(dict);
        return NULL;
    }

    tree = reconstruct_tree(dict);

    if (tree == NULL)
    {
        destroy_dict(dict);
        jep_destroy_bitstring(bs);
        return NULL;
    }

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

int jep_huff_write(jep_huff_code *hc, jep_byte_buffer *buffer)
{
    if (hc == NULL || buffer == NULL)
    {
        return 0;
    }

    if (hc->dict == NULL || hc->data == NULL)
    {
        return 0;
    }

    write_huff_dict(hc->dict, buffer);
    write_huff_data(hc->data, buffer);

    return 1;
}

void jep_destroy_huff_code(jep_huff_code *hc)
{
    if (hc == NULL)
    {
        return;
    }

    destroy_dict(hc->dict);
    destroy_tree(hc->tree);
    jep_destroy_bitstring(hc->data);

    free(hc);
}

/*-----------------------------------------------------------------*/
/*                 Memory Management Implementation                */
/*-----------------------------------------------------------------*/

static jep_huff_node *create_leaf_node()
{
    jep_huff_node *node;

    node = jep_alloc(jep_huff_node, 1);

    if (node == NULL)
    {
        return NULL;
    }

    node->leaf_1 = NULL;
    node->leaf_2 = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->sym.code = NULL;
    node->sym.byte = 0;
    node->sym.freq = 0;
    node->sym.weight = 0;
    node->sym.depth = 0;

    return node;
}

static void destroy_leaf_node(jep_huff_node *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->leaf_1 != NULL)
    {
        destroy_leaf_node(node->leaf_1);
    }

    if (node->leaf_2 != NULL)
    {
        destroy_leaf_node(node->leaf_2);
    }

    // We don't need to destroy the bitstrings here.
    // That's handled in destroy_dict.

    free(node);
}

static jep_huff_dict *create_dict(size_t count)
{
    jep_huff_dict *dict;
    jep_huff_sym *symbols;
    size_t i;

    dict = jep_alloc(jep_huff_dict, 1);

    if (dict == NULL)
    {
        return NULL;
    }

    dict->count = (uint32_t)count;

    symbols = jep_alloc(jep_huff_sym, count);

    if (symbols == NULL)
    {
        free(dict);
        return NULL;
    }

    for (i = 0; i < count; i++)
    {
        symbols[i].code = NULL;
    }

    dict->symbols = symbols;

    return dict;
}

static void destroy_dict(jep_huff_dict *dict)
{
    if (dict == NULL)
    {
        return;
    }

    uint32_t i;

    for (i = 0; i < dict->count; i++)
    {
        jep_destroy_bitstring(dict->symbols[i].code);
    }

    if (dict->symbols != NULL)
    {
        free(dict->symbols);
    }

    free(dict);
}

static jep_huff_tree *create_tree()
{
    jep_huff_tree *tree;

    tree = jep_alloc(jep_huff_tree, 1);

    if (tree == NULL)
    {
        return NULL;
    }

    tree->nodes = NULL;

    return tree;
}

static void destroy_tree(jep_huff_tree *tree)
{
    if (tree == NULL)
    {
        return;
    }

    jep_huff_node *node = tree->nodes;
    jep_huff_node *next = NULL;

    while (node != NULL)
    {
        next = node->next;
        destroy_leaf_node(node);
        node = next;
    }

    free(tree);
}

static jep_huff_code *create_huff_code()
{
    jep_huff_code *hc;
    jep_bitstring *data;

    hc = jep_alloc(jep_huff_code, 1);

    if (hc == NULL)
    {
        return NULL;
    }

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

/*-----------------------------------------------------------------*/
/*                  Tree Construction Implementation               */
/*-----------------------------------------------------------------*/

static void add_leaf_node(jep_huff_tree *tree, jep_huff_node *node)
{
    if (tree->nodes == NULL)
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
}

static void sort_leaf_nodes(jep_huff_tree *tree)
{
    if (tree == NULL || tree->nodes == NULL)
    {
        return;
    }

    int sorted;
    jep_huff_node **node;
    jep_huff_node *next;

    sorted = 0;

    while (!sorted)
    {
        sorted = 1;
        node = &(tree->nodes);

        // Use bubble sort to sort the list in order of increasing frequency.
        // Yeah, yeah, it's slow. Bite me.
        while (*node != NULL)
        {
            if ((*node)->next != NULL && (*node)->next->sym.freq < (*node)->sym.freq)
            {
                sorted = 0;

                next = (*node)->next;

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

static int construct_tree(jep_huff_tree *tree)
{
    jep_huff_node **node;
    jep_huff_node *parent;
    jep_huff_node *next;
    jep_huff_node *last_next;
    jep_huff_node *new_head;

    if (tree == NULL || tree->nodes == NULL)
    {
        return 0;
    }

    node = &(tree->nodes);

    while (*node != NULL && (*node)->next != NULL)
    {
        next = last_next = new_head = NULL;

        parent = jep_alloc(jep_huff_node, 1);

        if (parent == NULL)
        {
            return 0;
        }

        parent->leaf_1 = *node;
        parent->leaf_2 = (*node)->next;

        parent->sym.freq = parent->leaf_1->sym.freq;

        parent->sym.byte = 0;
        parent->sym.weight = 1;
        parent->sym.depth = 1;
        parent->sym.code = NULL;
        parent->next = NULL;
        parent->prev = NULL;

        if (parent->leaf_2 != NULL)
        {
            parent->sym.freq += parent->leaf_2->sym.freq;
            next = parent->leaf_2->next;
        }

        // The parent node should be placed right before the first
        // node that has a higher frequency that it.
        // So if the parent node has the highest frequency in the
        // list, it will be placed at the tail, and if it has the
        // lowest frequency, it will be placed at the head.

        last_next = next;
        new_head = next;

        while (next != NULL && next->sym.freq < parent->sym.freq)
        {
            last_next = next;
            next = next->next;
        }

        parent->next = next;

        if (next != NULL)
        {
            // The parent node is not the last node.
            parent->prev = next->prev;
            if (next->prev != NULL)
            {
                next->prev->next = parent;
            }
            next->prev = parent;
        }
        else if (next == NULL && last_next != NULL)
        {
            // The parent node is the last node.
            last_next->next = parent;
            parent->prev = last_next;
        }

        if (new_head == NULL || new_head == next)
        {
            *node = parent;
        }
        else
        {
            new_head->prev = NULL;
            *node = new_head;
        }
    }

    return 1;
}

static jep_huff_tree *reconstruct_tree(jep_huff_dict *dict)
{
    uint32_t i;
    jep_huff_tree *tree;
    jep_huff_node *root;
    jep_bitstring *bs;
    int res;

    tree = create_tree();

    if (tree == NULL)
    {
        return NULL;
    }

    root = create_leaf_node();

    if (root == NULL)
    {
        destroy_tree(tree);
        return NULL;
    }

    root->sym.weight = 1;

    add_leaf_node(tree, root);

    for (i = 0; i < dict->count; i++)
    {
        bs = dict->symbols[i].code;

        res = build_branch(&root, bs,
                           dict->symbols[i],
                           0, // current byte
                           0, // position in current byte
                           0  // current bit count
        );

        if (!res)
        {
            destroy_tree(tree);
            return NULL;
        }
    }

    return tree;
}

static int assign_bitcodes(jep_huff_tree *tree)
{
    jep_huff_node *node;
    jep_bitstring *bs;
    int res;

    if (tree == NULL)
    {
        return 0;
    }

    node = tree->nodes;

    node->sym.depth = 0;

    bs = jep_create_bitstring();

    if (node->sym.weight == 0)
    {
        if (!jep_push_bit(bs, 0))
        {
            jep_destroy_bitstring(bs);
            return 0;
        }
    }

    res = assign_bitcode(node, 0, bs);

    jep_destroy_bitstring(bs);

    return res;
}

static int assign_bitcode(
    jep_huff_node *node,
    uint32_t level,
    jep_bitstring *bs)
{
    if (node == NULL)
    {
        return 0;
    }

    if (node->sym.weight < 1)
    {
        node->sym.depth = level;
        if (!jep_concat_bits(node->sym.code, bs))
        {
            return 0;
        }
    }

    if (node->leaf_1 != NULL)
    {
        jep_push_bit(bs, 1);

        if (!assign_bitcode(node->leaf_1, level + 1, bs))
        {
            return 0;
        }

        if (!jep_pop_bit(bs))
        {
            return 0;
        }
    }

    if (node->leaf_2 != NULL)
    {
        jep_push_bit(bs, 0);

        if (!assign_bitcode(node->leaf_2, level + 1, bs))
        {
            return 0;
        }

        if (!jep_pop_bit(bs))
        {
            return 0;
        }
    }

    return 1;
}

static int build_branch(
    jep_huff_node **node,
    jep_bitstring *bs,
    jep_huff_sym data,
    jep_byte byte,
    uint32_t bit,
    uint32_t bit_count)
{
    jep_huff_node **leaf;

    if (bit_count == bs->bit_count)
    {
        if (*node == NULL)
        {
            *node = create_leaf_node();
            if (*node == NULL)
            {
                return 0;
            }
        }

        (*node)->sym = data;
        (*node)->sym.weight = 0;

        return 1;
    }

    if (bit == CHAR_BIT)
    {
        byte++;
        bit = 0;
    }

    if (bs->bytes[byte] & (1 << bit++))
    {
        leaf = &((*node)->leaf_1);
    }
    else
    {
        leaf = &((*node)->leaf_2);
    }

    if (*leaf == NULL)
    {
        *leaf = create_leaf_node();

        if (*leaf == NULL)
        {
            return 0;
        }

        (*leaf)->sym.weight = 1;
    }

    return build_branch(leaf, bs, data, byte, bit, ++bit_count);
}

/*-----------------------------------------------------------------*/
/*                     Buffer I/O Implementation                   */
/*-----------------------------------------------------------------*/

static void write_huff_dict(jep_huff_dict *dict, jep_byte_buffer *bb)
{
    uint32_t i, j;
    uint8_t u32buff[4];
    size_t pos;
    size_t size;

    jep_byte b;
    uint32_t bit_count;
    uint32_t byte_count;
    jep_byte current_bits;
    jep_byte *bytes;

    pos = 0;
    size = sizeof(jep_byte);

    write_to_buffer(&dict_begin, bb, size, &pos);

    for (i = 0; i < dict->count; i++)
    {
        // Gather the information about the current symbol.
        b = dict->symbols[i].byte;
        bit_count = dict->symbols[i].code->bit_count;
        byte_count = dict->symbols[i].code->byte_count;
        current_bits = dict->symbols[i].code->current_bits;
        bytes = dict->symbols[i].code->bytes;

        write_to_buffer(&dict_byte, bb, size, &pos);
        write_to_buffer(&b, bb, 1, &pos);

        write_to_buffer(&dict_code, bb, size, &pos);

        jep_split_u32(bit_count, u32buff);
        write_to_buffer(u32buff, bb, size * 4, &pos);

        jep_split_u32(byte_count, u32buff);
        write_to_buffer(u32buff, bb, size * 4, &pos);

        write_to_buffer(&current_bits, bb, size, &pos);

        for (j = 0; j < byte_count; j++)
        {
            write_to_buffer(&(bytes[j]), bb, size, &pos);
        }
    }

    write_to_buffer(&dict_end, bb, sizeof(jep_byte), &pos);
}

static void write_huff_data(jep_bitstring *data, jep_byte_buffer *bb)
{
    uint32_t i;
    uint8_t u32buff[4];
    size_t pos;
    size_t size;

    uint32_t bit_count;
    uint32_t byte_count;
    jep_byte current_bits;

    pos = 0;
    size = sizeof(jep_byte);

    bit_count = data->bit_count;
    byte_count = data->byte_count;
    current_bits = data->current_bits;

    write_to_buffer(&data_begin, bb, sizeof(jep_byte), &pos);

    jep_split_u32(bit_count, u32buff);
    write_to_buffer(u32buff, bb, size * 4, &pos);

    jep_split_u32(byte_count, u32buff);
    write_to_buffer(u32buff, bb, size * 4, &pos);

    write_to_buffer(&current_bits, bb, size, &pos);

    for (i = 0; i < data->byte_count; i++)
    {
        write_to_buffer(&(data->bytes[i]), bb, size, &pos);
    }

    write_to_buffer(&data_end, bb, size, &pos);
}

static jep_huff_dict *read_huff_dict(jep_byte_buffer *data, size_t *pos)
{
    jep_huff_dict *dict;
    size_t cap;
    size_t new_cap;
    size_t count;
    int res;
    jep_huff_sym sym;
    jep_huff_sym *syms;
    jep_byte u32buff[4];
    uint32_t u32;
    jep_byte byte;
    size_t size;

    uint32_t bit_count;
    uint32_t byte_count;
    jep_byte *bytes;

    cap = 10;
    dict = create_dict(sizeof(jep_huff_sym) * cap);
    count = 0;
    res = 1;
    u32 = 0;
    byte = 0;
    size = sizeof(jep_byte);

    if (dict == NULL)
    {
        return NULL;
    }

    res = read_from_buffer(&byte, data, size, pos);

    if (!res || byte != dict_begin)
    {
        destroy_dict(dict);
        return NULL;
    }

    while (res)
    {
        res = read_from_buffer(&byte, data, size, pos);

        if (res && byte == dict_byte)
        {
            if ((res = read_from_buffer(&byte, data, size, pos)))
            {
                sym.byte = byte;
            }
        }
        else if (res && byte == dict_code)
        {
            sym.code = jep_create_bitstring();

            if (sym.code == NULL)
            {
                destroy_dict(dict);
                return NULL;
            }

            // Destroy the bytes since we'll recreate them later.
            free(sym.code->bytes);

            res = read_from_buffer(u32buff, data, size * 4, pos);
            jep_build_u32(u32, u32buff);
            bit_count = u32;

            res = read_from_buffer(u32buff, data, size * 4, pos);
            jep_build_u32(u32, u32buff);
            byte_count = u32;

            res = read_from_buffer(&byte, data, size, pos);

            bytes = jep_alloc(jep_byte, byte_count);

            res = read_from_buffer(bytes, data, byte_count, pos);

            if (count >= cap)
            {
                new_cap = cap + cap / 2;

                syms = resym(dict->symbols, new_cap);

                if (syms == NULL)
                {
                    dict->symbols = NULL;
                    destroy_dict(dict);
                    return NULL;
                }

                dict->symbols = syms;
                cap = new_cap;
            }

            sym.code->bit_count = bit_count;
            sym.code->byte_count = byte_count;
            sym.code->bytes = bytes;

            dict->symbols[count++] = sym;
        }
        else if (byte == dict_end)
        {
            res = 0;
        }
    }

    if (count < cap)
    {
        syms = resym(dict->symbols, count);

        if (syms == NULL)
        {
            dict->symbols = NULL;
            destroy_dict(dict);
            return NULL;
        }

        dict->symbols = syms;
    }

    dict->count = (uint32_t)count;

    return dict;
}

static jep_bitstring *read_huff_data(jep_byte_buffer *data, size_t *pos)
{
    jep_bitstring *bs;
    jep_byte u32buff[4];
    uint32_t u32;
    jep_byte byte;
    size_t size;

    bs = jep_create_bitstring();

    // Dispose of the byte array since it will be created later.
    free(bs->bytes);

    u32 = 0;
    byte = 0;
    size = sizeof(jep_byte);

    read_from_buffer(&byte, data, size, pos);

    if (byte == data_begin)
    {
        read_from_buffer(u32buff, data, size * 4, pos);
        jep_build_u32(u32, u32buff);
        bs->bit_count = u32;

        read_from_buffer(u32buff, data, size * 4, pos);
        jep_build_u32(u32, u32buff);
        bs->byte_count = u32;

        read_from_buffer(&byte, data, size, pos);
        bs->current_bits = byte;

        bs->bytes = jep_alloc(jep_byte, bs->byte_count);

        read_from_buffer(bs->bytes, data, bs->byte_count, pos);
    }

    return bs;
}

static int read_from_buffer(jep_byte *dest,
                            jep_byte_buffer *src,
                            size_t n,
                            size_t *pos)
{
    size_t i, j;
    size_t start;
    int result;

    if (src == NULL || src->buffer == NULL || dest == NULL)
    {
        return 0;
    }

    start = *pos;
    result = 0;

    if (*pos >= src->size)
    {
        return 0;
    }

    for (i = start, j = 0; i < start + n && i < src->size; i++)
    {
        dest[j++] = src->buffer[i];
        (*pos)++;
        result++;
    }

    return result;
}

static int write_to_buffer(const jep_byte *src,
                           jep_byte_buffer *dest,
                           size_t n,
                           size_t *pos)
{
    size_t i;
    int result;

    if (dest == NULL || dest->buffer == NULL || src == NULL)
    {
        return 0;
    }

    result = 0;

    if (*pos > dest->size)
    {
        return 0;
    }

    for (i = 0; i < n; i++)
    {
        jep_append_byte(dest, src[i]);
        (*pos)++;
        result++;
    }

    return result;
}
