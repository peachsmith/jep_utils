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
        return NULL;

    huff = create_huff_code();
    data = jep_create_bitstring();
    tree = create_tree();

    // Check for failure to create any of the components
    if (huff == NULL || data == NULL || tree == NULL)
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    // Prepare the dictionary.
    for (i = 0; i < UCHAR_MAX + 1; i++)
    {
        bytes[i].b = (jep_byte)i;
        bytes[i].f = 0;
        bytes[i].w = 0;
        bytes[i].n = 1;
        bytes[i].code = NULL;
    }

    // Determine the unique bytes.
    // Any bitstrings created here will be destroyed
    // when the dictionary is destroyed.
    unique = 0;
    for (i = 0; i < raw->size; i++)
    {
        if (bytes[raw->buffer[i]].f == 0)
        {
            unique++;
            bytes[raw->buffer[i]].code = jep_create_bitstring();

            // Check for failure to create bitstring
            if (bytes[raw->buffer[i]].code == NULL)
            {
                jep_destroy_huff_code(huff);
                jep_destroy_bitstring(data);
                destroy_tree(tree);
                return NULL;
            }
        }
        bytes[raw->buffer[i]].f++;
    }

    // Create Huffman leaf nodes.
    // Any nodes created here will be destroyed
    // when the tree is destroyed.
    for (i = 0; i < UCHAR_MAX + 1; i++)
    {
        if (bytes[i].f > 0)
        {
            jep_huff_node *node = create_leaf_node();

            // Check for failure to create leaf node
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

    // Sort the list of leaf nodes.
    sort_leaf_nodes(tree);

    // Construct the tree and check for failure.
    if (!construct_tree(tree))
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    // Assign bitcodes and check for failure.
    if (!assign_bitcodes(tree))
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    // Create the bitcode dictionary.
    dict = create_dict(unique);

    // Check for failure to create the dictionary.
    if (dict == NULL)
    {
        jep_destroy_huff_code(huff);
        jep_destroy_bitstring(data);
        destroy_tree(tree);
        return NULL;
    }

    // Populate the bitcode dictionary.
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

    // Encode the data
    for (i = 0; i < raw->size; i++)
    {
        for (j = 0; j < unique; j++)
        {
            if (dict->symbols[j].b == raw->buffer[i])
            {
                // Add bits and check for failure
                if (!jep_add_bits(data, dict->symbols[j].code))
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

    // Write the Huffman Coding data to the output buffer.
    jep_huff_write(huff, encoded);

    return encoded;
}

jep_byte_buffer *jep_huff_decode(jep_byte_buffer *encoded)
{
    jep_byte_buffer *raw; // The decoded data
    jep_huff_code *hc;    // Huffman Coding data
    jep_huff_node *root;  // The root node of the Huffman tree
    jep_huff_node *leaf;  // The current leaf node
    uint32_t byte;        // Current byte of a bitstring
    uint32_t bit;         // Current bit of a byte in a bitstring
    uint32_t i;           // Index

    if (encoded == NULL)
        return NULL;

    hc = jep_huff_read(encoded);

    if (hc == NULL)
        return NULL;

    raw = jep_create_byte_buffer();

    if (raw == NULL)
    {
        jep_destroy_huff_code(hc);
        return NULL;
    }

    root = hc->tree->nodes;
    leaf = root;
    bit = byte = 0;

    // Traverse the Huffman tree.
    // Starting from the root node,
    // if the current bit in the bitstring is 1, we move on
    // to leaf 1 of the current node, otherwise we move to leaf 2.
    // Upon reaching the last node of a branch, we add the byte value
    // stored in that node to the output buffer.
    for (i = 0; i <= hc->data->bit_count; i++)
    {
        // Update the bitstring position variables
        // checking the current bit advances the bit counter.
        // Once that has reached CHAR_BIT, we reset it and
        // advance the byte counter.
        if (bit == CHAR_BIT)
        {
            byte++;
            bit = 0;
        }

        if (leaf->sym.w == 0)
        {
            // Attempt to add the byte to the output
            // buffer and check for failure.
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
            // Determine which leaf node to visit next
            // and advance the bit counter
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

    // Read the dictionary.
    dict = read_huff_dict(raw, &pos);

    if (dict == NULL)
        return NULL;

    // Read the data
    bs = read_huff_data(raw, &pos);

    if (bs == NULL)
    {
        destroy_dict(dict);
        return NULL;
    }

    // Reconstruct a Huffman tree
    tree = reconstruct_tree(dict);

    if (tree == NULL)
    {
        destroy_dict(dict);
        jep_destroy_bitstring(bs);
        return NULL;
    }

    // Create and populate the Huffman Coding context
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
        return 0;

    if (hc->dict == NULL || hc->data == NULL)
        return 0;

    write_huff_dict(hc->dict, buffer);
    write_huff_data(hc->data, buffer);

    return 1;
}

void jep_destroy_huff_code(jep_huff_code *hc)
{
    if (hc == NULL)
        return;

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

static void destroy_leaf_node(jep_huff_node *node)
{
    if (node == NULL)
        return;

    if (node->leaf_1 != NULL)
        destroy_leaf_node(node->leaf_1);

    if (node->leaf_2 != NULL)
        destroy_leaf_node(node->leaf_2);

    // Disposal of bitstrings is handled in destroy_dict.

    free(node);
}

static jep_huff_dict *create_dict(size_t count)
{
    jep_huff_dict *dict;
    jep_huff_sym *symbols;
    size_t i;

    dict = jep_alloc(jep_huff_dict, 1);

    if (dict == NULL)
        return NULL;

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
        return;

    uint32_t i;

    for (i = 0; i < dict->count; i++)
        jep_destroy_bitstring(dict->symbols[i].code);

    if (dict->symbols != NULL)
        free(dict->symbols);

    free(dict);
}

static jep_huff_tree *create_tree()
{
    jep_huff_tree *tree;

    tree = jep_alloc(jep_huff_tree, 1);

    if (tree == NULL)
        return NULL;

    tree->nodes = NULL;

    return tree;
}

static void destroy_tree(jep_huff_tree *tree)
{
    if (tree == NULL)
        return;

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

/*-----------------------------------------------------------------*/
/*                  Tree Construction Implementation               */
/*-----------------------------------------------------------------*/

static void add_leaf_node(jep_huff_tree *tree, jep_huff_node *node)
{
    // This is your run-of-the-mill linked list insertion.
    // If list is empty, the new node becomes the first node in the list.
    // If the list is not empty, the new node gets added at the tail.
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
        return;

    int sorted;           // Whether or not the list is sorted
    jep_huff_node **node; // The head of the list
    jep_huff_node *next;  // The next node in the list

    sorted = 0;

    // Attempt to sort the list until it it sorted.
    while (!sorted)
    {
        sorted = 1;
        node = &(tree->nodes);

        // Use bubble sort to sort the list in order
        // of increasing frequency.
        // This may be a tremendous source of inefficiency.
        while (*node != NULL)
        {
            if ((*node)->next != NULL && (*node)->next->sym.f < (*node)->sym.f)
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
    jep_huff_node **node;     // The current node
    jep_huff_node *par;       // A parent node for two leaf nodes
    jep_huff_node *next;      // The next node in the list
    jep_huff_node *last_next; // The old next node
    jep_huff_node *new_head;  // The new head of the list

    // Ensure that we have a tree and that it has leaves.
    if (tree == NULL || tree->nodes == NULL)
        return 0;

    // Grab a reference to the first node in the list.
    node = &(tree->nodes);

    // Loop through the list of nodes and
    while (*node != NULL && (*node)->next != NULL)
    {
        // Initialize the node pointers.
        next = last_next = new_head = NULL;

        // Create a new parent node to represent the sum
        // of the next two nodes.
        par = jep_alloc(jep_huff_node, 1);

        if (par == NULL)
            return 0;

        // The current node and the next node become
        // leaf 1 and leaf 2 of the new parent node respectively.
        par->leaf_1 = *node;
        par->leaf_2 = (*node)->next;

        // Initialize the frequency of the parent node
        // to be that of the current node.
        par->sym.f = par->leaf_1->sym.f;

        par->sym.b = 0;
        par->sym.w = 1;
        par->sym.n = 1;
        par->sym.code = NULL;
        par->next = NULL;
        par->prev = NULL;

        // If the next node is not NULL,
        // add its frequency to the parent node's frequency,
        // and grab a reference to the
        if (par->leaf_2 != NULL)
        {
            par->sym.f += par->leaf_2->sym.f;
            next = par->leaf_2->next;
        }

        // The parent node should be placed right before the first
        // node that has a higher frequency that it.
        // So if the parent node has the highest frequency in the
        // list, it will be placed at the tail, and if it has the
        // lowest frequency, it will be placed at the head.

        last_next = next;
        new_head = next;

        // Determine the value of the parent node's "next" pointer.
        while (next != NULL && next->sym.f < par->sym.f)
        {
            last_next = next;
            next = next->next;
        }

        par->next = next;

        // Determine the value of the parent node's "prev" pointer.
        if (next != NULL)
        {
            // The parent node is not the last node.
            par->prev = next->prev;
            if (next->prev != NULL)
            {
                next->prev->next = par;
            }
            next->prev = par;
        }
        else if (next == NULL && last_next != NULL)
        {
            // The parent node is the last node.
            last_next->next = par;
            par->prev = last_next;
        }

        // Determine the new value of the current node pointer.
        // If the parent node was inserted at the head of the list,
        // then the new current node is the parent node. Otherwise,
        // the next node in the list becomes the current one.
        if (new_head == NULL || new_head == next)
        {
            *node = par;
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
    uint32_t i;          // Index
    jep_huff_tree *tree; // A Huffman tree
    jep_huff_node *root; // The root node of a Huffman tree
    jep_bitstring *bs;   // The bitstring for a Huffman symbol
    int res;             // The result of building a branch

    // Create the tree.
    tree = create_tree();

    if (tree == NULL)
        return NULL;

    // Create the root node.
    root = create_leaf_node();

    if (root == NULL)
    {
        destroy_tree(tree);
        return NULL;
    }

    root->sym.w = 1;

    // Add the root node to the tree.
    add_leaf_node(tree, root);

    // Loop through the symbols in the dictionary
    // and build each branch of the tree based on
    // the bit code.
    for (i = 0; i < dict->count; i++)
    {
        bs = dict->symbols[i].code;

        res = build_branch(&root, bs,
                           dict->symbols[i],
                           0, // current byte
                           0, // position in current byte
                           0  // current bit count
        );

        // If we failed to build a branch,
        // destroy what we've built so far and leave.
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
    jep_huff_node *node; // The root node of the Huffman tree
    jep_bitstring *bs;   // A bitstring to hold the bit codes
    int res;             // The result of assigning the bit codes

    // Ensure that the tree exists.
    if (tree == NULL)
        return 0;

    node = tree->nodes;

    node->sym.n = 0;

    // Create the bitstring.
    bs = jep_create_bitstring();

    if (node->sym.w == 0)
    {
        if (!jep_add_bit(bs, 0))
        {
            jep_destroy_bitstring(bs);
            return 0;
        }
    }

    // Assign the bitcodes to each node of the tree.
    res = assign_bitcode(node, 0, bs);

    // Destroy the bitstring.
    jep_destroy_bitstring(bs);

    return res;
}

static int assign_bitcode(
    jep_huff_node *node,
    uint32_t level,
    jep_bitstring *bs)
{
    // Ensure that the node actually exists.
    if (node == NULL)
        return 0;

    // If the weight of the current symbol is less than 1,
    // add the bits to the symbol's bitstring.
    if (node->sym.w < 1)
    {
        node->sym.n = level;
        if (!jep_add_bits(node->sym.code, bs))
            return 0;
    }

    // If the first leaf exists,
    // add a bit with the value of 1 to the current bitstring,
    // make a recursive call to this function to progress to
    // the next level, then remove the bit.
    if (node->leaf_1 != NULL)
    {
        jep_add_bit(bs, 1);

        if (!assign_bitcode(node->leaf_1, level + 1, bs))
            return 0;

        if (!jep_pop_bit(bs))
            return 0;
    }

    // If the first leaf exists,
    // add a bit with the value of 0 to the current bitstring,
    // make a recursive call to this function to progress to
    // the next level, then remove the bit.
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
    jep_huff_node **node,
    jep_bitstring *bs,
    jep_huff_sym data,
    jep_byte byte,
    uint32_t bit,
    uint32_t bit_count)
{
    jep_huff_node **leaf; // The current leaf node

    // If the current bit count matches the number of bits
    // in the current bitstring, then we've reached the
    // end of a branch.
    if (bit_count == bs->bit_count)
    {
        // Create the leaf node if it doesn't already exist.
        if (*node == NULL)
        {
            *node = create_leaf_node();
            if (*node == NULL)
                return 0;
        }

        // Populate the symbol data of the leaf node.
        (*node)->sym = data;
        (*node)->sym.w = 0;

        return 1;
    }

    // If we've exceeded the number of bits in a byte,
    // increment the byte counter and reset the bit counter.
    if (bit == CHAR_BIT)
    {
        byte++;
        bit = 0;
    }

    // If the current bit is a 1, then the next
    // node in the branch will be leaf_1,
    // otherwise it will be leaf_2.
    if (bs->bytes[byte] & (1 << bit++))
    {
        leaf = &((*node)->leaf_1);
    }
    else
    {
        leaf = &((*node)->leaf_2);
    }

    // Create the next leaf node if it doesn't already exist.
    if (*leaf == NULL)
    {
        *leaf = create_leaf_node();

        if (*leaf == NULL)
            return 0;

        (*leaf)->sym.w = 1;
    }

    // Make a recursive call to this function to move to the next node.
    return build_branch(leaf, bs, data, byte, bit, ++bit_count);
}

/*-----------------------------------------------------------------*/
/*                     Buffer I/O Implementation                   */
/*-----------------------------------------------------------------*/

static void write_huff_dict(jep_huff_dict *dict, jep_byte_buffer *bb)
{
    uint32_t i, j;      // Indices
    uint8_t u32buff[4]; // Buffer to hold the bytes of a uint32_t
    size_t pos;         // Position in the output stream
    size_t s;           // The size of 1 byte. This should always be 1.

    jep_byte b;            // Byte value of the current symbol
    uint32_t bit_count;    // Number of bits in the bitstring
    uint32_t byte_count;   // Number of bytes in the bitstring
    jep_byte current_bits; // Number of occupied bits in the last byte
    jep_byte *bytes;       // Bytes in the bitstring

    pos = 0;
    s = sizeof(jep_byte);

    // Write the dict_begin metadata to signify the beginning
    // of a dictionary.
    write_to_buffer(&dict_begin, bb, s, &pos);

    // Loop through the symbols in the dictionary and write
    // their data to the output buffer.
    // Since the bit count and byte count are unsigned 32-bit integers,
    // we split them into four 8-bit bytes before writing.
    for (i = 0; i < dict->count; i++)
    {
        // Gather the information about the current symbol.
        b = dict->symbols[i].b;
        bit_count = dict->symbols[i].code->bit_count;
        byte_count = dict->symbols[i].code->byte_count;
        current_bits = dict->symbols[i].code->current_bits;
        bytes = dict->symbols[i].code->bytes;

        // Write the dict_byte metadata to signify that
        // we're about to write the byte value of the
        // current symbol.
        // Then, write the symbol's byte value.
        write_to_buffer(&dict_byte, bb, s, &pos);
        write_to_buffer(&b, bb, 1, &pos);

        // Write the dict_code metadata to signify that
        // we're about to write bitstring information.
        write_to_buffer(&dict_code, bb, s, &pos);

        // Write the bit count of the bitstring.
        jep_split_u32(bit_count, u32buff);
        write_to_buffer(u32buff, bb, s * 4, &pos);

        // Write the byte count of the bitstring.
        jep_split_u32(byte_count, u32buff);
        write_to_buffer(u32buff, bb, s * 4, &pos);

        // Write the number of currently occupied bits in
        // the last byte.
        write_to_buffer(&current_bits, bb, s, &pos);

        // Write the bytes of the bitstring data.
        for (j = 0; j < byte_count; j++)
        {
            write_to_buffer(&(bytes[j]), bb, s, &pos);
        }
    }

    // Write the dict_end metadata to signify that we've finished
    // writing the dictionary.
    write_to_buffer(&dict_end, bb, sizeof(jep_byte), &pos);
}

static void write_huff_data(jep_bitstring *data, jep_byte_buffer *bb)
{
    uint32_t i;         // Index
    uint8_t u32buff[4]; // Buffer to hold the bytes of a uint32_t
    size_t pos;         // Position in the output stream
    size_t s;           // The size of 1 byte. This should always be 1.

    uint32_t bit_count;    // Number of bits in the bitstring
    uint32_t byte_count;   // Number of bytes in the bitstring
    jep_byte current_bits; // Number of occupied bits in the last byte

    pos = 0;
    s = sizeof(jep_byte);

    bit_count = data->bit_count;
    byte_count = data->byte_count;
    current_bits = data->current_bits;

    // Write the data_begin metadata to signify that we're
    // about to write the encoded bitstring data.
    write_to_buffer(&data_begin, bb, sizeof(jep_byte), &pos);

    // Write the bit count.
    jep_split_u32(bit_count, u32buff);
    write_to_buffer(u32buff, bb, s * 4, &pos);

    // Write the byte count.
    jep_split_u32(byte_count, u32buff);
    write_to_buffer(u32buff, bb, s * 4, &pos);

    // Write the number of bits occupied in the last byte.
    write_to_buffer(&current_bits, bb, s, &pos);

    // Write the bitstring data.
    for (i = 0; i < data->byte_count; i++)
    {
        write_to_buffer(&(data->bytes[i]), bb, s, &pos);
    }

    // Write the data_end metadata to signify that we've finished
    // writing the data.
    write_to_buffer(&data_end, bb, s, &pos);
}

static jep_huff_dict *read_huff_dict(jep_byte_buffer *data, size_t *pos)
{
    jep_huff_dict *dict; // The dictionary to be read from the buffer
    size_t cap;          // Capacity of the dictionary
    size_t new_cap;      // New capacity for resizing the dictionary
    size_t count;        // Number of symbols in the dictionary
    int res;             // Result of read operations
    jep_huff_sym sym;    // An individual symbol
    jep_huff_sym *syms;  // Pointer used for reallocation
    jep_byte u32buff[4]; // Buffer for holding an unsigned 32-bit integer
    uint32_t u32;        // An unsigned 32-bit integer
    jep_byte b;          // An unsigned 8-bit integer
    size_t s;            // The size of 1 byte. This should always be 1.

    uint32_t bit_count;  // Number of bits in the bitstring
    uint32_t byte_count; // Number of bytes in the bitstring
    jep_byte *bytes;     // Bytes in the bitstring

    cap = 10;
    dict = create_dict(sizeof(jep_huff_sym) * cap);
    count = 0;
    res = 1;
    u32 = 0;
    b = 0;
    s = sizeof(jep_byte);

    // Ensure that we successfully created the dictionary.
    if (dict == NULL)
        return NULL;

    // Read the first byte from the buffer.
    res = read_from_buffer(&b, data, s, pos);

    // Verify that the first byte is the dict_begin metadata.
    if (!res || b != dict_begin)
    {
        destroy_dict(dict);
        return NULL;
    }

    // As long as there are bytes remaining to be read,
    // read them from the buffer and insert them into the dictionary.
    while (res)
    {
        // Read the next byte.
        res = read_from_buffer(&b, data, s, pos);

        if (res && b == dict_byte)
        {
            // If the previous byte was the dict_byte metadata,
            // then the next byte will be the value of a symbol.
            if ((res = read_from_buffer(&b, data, s, pos)))
                sym.b = b;
        }
        else if (res && b == dict_code)
        {
            // If the previous byte was the dict_code metadata,
            // then the following bytes will be a bistring.
            // The bit count and byte count are 32-bit
            // unsigned integers, so they will need to be
            // reconstructed from a sequence of four bytes.

            // Create the bitstring for the current symbol.
            sym.code = jep_create_bitstring();

            // Ensure that the bitstring was created.
            if (sym.code == NULL)
            {
                destroy_dict(dict);
                return NULL;
            }

            // Destroy the bytes since we'll recreate them later.
            free(sym.code->bytes);

            // Read the bit count.
            res = read_from_buffer(u32buff, data, s * 4, pos);
            jep_build_u32(u32, u32buff);
            bit_count = u32;

            // Read the byte count.
            res = read_from_buffer(u32buff, data, s * 4, pos);
            jep_build_u32(u32, u32buff);
            byte_count = u32;

            // Read the number of bits occupied in the last byte.
            res = read_from_buffer(&b, data, s, pos);

            // Allocate a new array of bytes.
            bytes = jep_alloc(jep_byte, byte_count);

            // Read the bytes of the bitstring.
            res = read_from_buffer(bytes, data, byte_count, pos);

            // Resize the dictionary if necessary.
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

            // Populate members of the symbol structure.
            sym.code->bit_count = bit_count;
            sym.code->byte_count = byte_count;
            sym.code->bytes = bytes;

            // Add the symbol to the dictionary.
            dict->symbols[count++] = sym;
        }
        else if (b == dict_end)
        {
            // If the previous byte was the dict_end metadata,
            // break out of the loop.
            res = 0;
        }
    }

    // If we allocated more memory than necessary,
    // resize the dictionary's symbol array to have
    // an appropriate length.
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
    jep_bitstring *bs;   // A bitstring to hold the data
    jep_byte u32buff[4]; // Buffer for holding an unsigned 32-bit integer
    uint32_t u32;        // An unsigned 32-bit integer
    jep_byte b;          // An unsigned 8-bit integer
    size_t s;            // The size of 1 byte. This should always be 1.

    // Create the bitstring.
    bs = jep_create_bitstring();

    // Dispose of the byte array since it will be created later.
    free(bs->bytes);

    u32 = 0;
    b = 0;
    // res = 1;
    s = sizeof(jep_byte);

    // Read the first byte from the buffer.
    read_from_buffer(&b, data, s, pos);

    // Verify that the first byte was the data_begin metadata.
    if (b == data_begin)
    {
        // Read the bit count.
        read_from_buffer(u32buff, data, s * 4, pos);
        jep_build_u32(u32, u32buff);
        bs->bit_count = u32;

        // Read the byte count.
        read_from_buffer(u32buff, data, s * 4, pos);
        jep_build_u32(u32, u32buff);
        bs->byte_count = u32;

        // Read the number of bits occupied in the last byte.
        read_from_buffer(&b, data, s, pos);
        bs->current_bits = b;

        // Allocate an array of bytes.
        bs->bytes = jep_alloc(jep_byte, bs->byte_count);

        // Read the bitstring data.
        read_from_buffer(bs->bytes, data, bs->byte_count, pos);
    }

    return bs;
}

static int read_from_buffer(jep_byte *dest,
                            jep_byte_buffer *src,
                            size_t n,
                            size_t *pos)
{
    size_t i, j;  // Indices
    size_t start; // Position in output stream
    int result;   // Number of bytes written to output buffer

    // Ensure that the source and destination are not NULL.
    if (src == NULL || src->buffer == NULL || dest == NULL)
        return 0;

    start = *pos;
    result = 0;

    // Ensure that the current position is less than
    // the source size.
    if (*pos >= src->size)
        return 0;

    // Copy each byte from the source buffer into the destination array.
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
    size_t i;   // Index
    int result; // Number of bytes written to output buffer

    // Ensure that the source and destination are not NULL.
    if (dest == NULL || dest->buffer == NULL || src == NULL)
        return 0;

    result = 0;

    // Ensure that the current position is not greater
    // than the size of the destination buffer.
    if (*pos > dest->size)
        return 0;

    // Copy each byte from the source array into the destination buffer.
    for (i = 0; i < n; i++)
    {
        jep_append_byte(dest, src[i]);
        (*pos)++;
        result++;
    }

    return result;
}
