# Background

## Huffman Encoding:

Huffman Encoding is a file compression technique that is often used to compress JPEG and MP3 files. This encoding uses fewer bits to encode characters that occur frequently and more bits to encode characters that occur less frequently. *This is the basic idea behind Huffman coding: to use fewer bits for characters that occur more frequently.*

To start, I count the occurrence of every character in the string/data that we want to encode. Store occurrence of each character and sort them by their frequency. I begin with a forest of trees. All trees initially consist of one node, with the weight of the tree equal to the weight of the character in the node. Characters that occur most frequently have the highest weights. Characters that occur least frequently have the smallest weights. 

### Example :
I use the string `go go gophers` as an example. Initially we have the forest shown below. The nodes are shown with a weight/count that represents the number of times the node's character occurs. We will use sorted linked list whose nodes will point to the respective root tree node. Thus we are representing order of the tree nodes by putting them in an ordered linked list. Each node in the list points to the indivisual tree node. Node pointing to tree node ‘e’ is the head of the list.

![gophers1.jpg](http://i.imgur.com/Edfn4FV.jpg)

We maintain ordered list of items arranged according to their weights. If the two nodes of the list have same weight, a leaf node(associated with an ASCII character) is ordered first. If both nodes are leaf nodes, they are ordered according to their ASCII coding. If both nodes are non-leaf nodes, they are ordered according to the creation times of the nodes.

We always pick the first two items in the list, here, nodes for characters 'e' and 'h'. We create a new tree whose root is weighted by the sum of the weights chosen. Although the newly created node has the same weight as Space, it is ordered after Space in the list because Space is an ASCII character. 
 

![gophers1.jpg](http://i.imgur.com/ESrba5k.jpg)


Choosing the first two (minimal) nodes in the list yields another tree with weight 2 as shown below. There are now six trees in the forest of trees that will eventually build an encoding tree.

![gophers2.jpg](http://i.imgur.com/2Hv3lSC.jpg)

Again we must choose the first two (minimal) nodes in the list. The lowest weight is the 'e'-node/tree with weight equal to 1. There are three trees with weight 2; the one chosen corresponds to an ASCII character because of the way we order the nodes in the list. The new tree has a weight of 3, which will be placed last in the linked list according to our ordering strategy.
 

![gophers3.jpg](http://i.imgur.com/Frm71a5.jpg)


Now there are two trees with weight equal to 2. These are joined into a new tree whose weight is 4. There are four trees left, one whose weight is 4 and three with a weight of 3.


![gophers4.jpg](http://i.imgur.com/8Je9svr.jpg)

The first two minimal (weight 3) trees in the list are joined into a tree whose weight is 6. There are three trees left.


![gophers5.jpg](http://i.imgur.com/XGd3VTS.jpg)

The minimal trees have weights of 3 and 4; these are joined into a tree with weight 7.


![gophers6.jpg](http://i.imgur.com/i8e7HiQ.jpg)


Finally, the last two trees are joined into a final tree whose weight is 13, the sum of the two weights 6 and 7. This tree is the tree we used to illustrate Huffman coding above.

![gophers7.jpg](http://i.imgur.com/ZEQ9KVM.jpg)

To build the code for every character, we start from the root node and travel to every leaf node corresoinding to the character. We denote every left branch as '0' and every right branch as '1'. (Note that '0' and '1' are characters and not bits). We obtain the code for every character according as shown in the table mentioned below :


							| Character | Binary code |
							| --------- |  ---------- |
							|  '  '     |  101        | 
							|  'e'      |  1100       | 
							|  'g'      |  00         |
							|  'h'      |  1101       |
							|  'o'      |  01         | 
							|  'p'      |  1110       |
							|  'r'      |  1111       | 
							|  's'      |  100        |

## Header Information
To compress the string/file, we read one character at a time and write the sequence of bits that encodes each character.
We must also store some initial information in the compressed file that will be used by the uncompression/unhuffing program. 
Basically we must store the tree used to compress the original file. This tree is used by the uncompression program. We can store the tree at the beginning of the file. We will do a pre-order traversal of the binary tree generated in the previous step, writing each node visited. We must differentiate leaf nodes from internal/non-leaf nodes. One way to do this is write a single bit for each node, say 1 for leaf and 0 for non-leaf. For leaf nodes, you will also need to write the ASCII character stored. For non-leaf nodes there's no information that needs to be written, just the bit that indicates there's an internal node. There should also be a 0 bit at the end to signify end of the tree information.

### Example :
Encoding tree for string go go gophers can be represented as following using pre-order traversal :

`001g1o001s1 001e1h01p1r0`

Here bit `1` is written before each leaf node followed by character stored at the leaf node, bit `0` is written for a non leaf node and 0 is added at the end to signify end of the tree.

In particular, it is not possible to write just one single bit to a file, minimum unit of memory you can access in C is a byte. We should accumulate 8 bits in a buffer and then write it to the file. If we are writing  the above data for tree, first three bits in the first byte would be 001 followed by 5 most significant bits of ASCII value of character 'g'. Remaining 3 bits of character 'g' would go as first three most significant bits of the second byte.

If the binary representation of ASCII value of character `'g'` is `01100111` and `'o'` is `01101111`, first two bytes would be

`00101100 11110110`

(note that space is provided between two bytes only for clarity; in the file itself all of those bits would be consecutive.)

The last byte may not contain 8 meaningful bits, you should pad remaining it with `0` bits in remaining places.

# Code Logic

The code creates an array `asciiCount` of size `ASCII_SIZE` which stores the count of each character at the index of that character’s ASCII value. Value of `ASCII_SIZE` is `256`, defined in `huffman.h` file.

e.g if input file contains character `‘a’` three times and ASCII value of `‘a’` is 97, `asciiCount[97]` will store value `3`.

To create and maintain the sorted list of trees according to their weight, a linked list is implimented, with each node containing the count and and an address that points to the root of binary tree.I only create tree nodes for characters having non zero count in array `asciiCount`.

I use bit-wise operations for generating the header information. I use "`<<`", "`>>`" , "`|`" and "`&`" bit-wise operators and different masks to split the character into chunks of bits desired and shift operators to move the extracted bits to correct positions.

The program reads an input file (its filename is provided to the program as `argv[1]`) and produces three output files :

**Output file 1** consists only the characters that appear in the input file and their count, separated by a ‘:’ character. Characters are sorted in the ascending order according to the count. If two characters have the same number of occurrence, character with the smaller ASCII value appear first. 

**Output file 2** consists of characters and stream of 0 and 1 (‘0’ and ‘1’ are characters and not bits) corresponding to the Huffman code of the character, seperated by ':' character. 

**Output file 3** contains the header information which is the representation of the encoding binary tree using pre-order traversal. The last byte in the output file may need padding bits of 0 in least significant positions.
