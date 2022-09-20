# Welcome to My Blockchain
***

## Task

TODO - My Blockchain 

## Description

Blockchain is a command that allows for the creation and management of a blockchain. When the program starts (it loads a backup if there is one) then a prompt appears. This prompt allows to execute commands. When the commands are successful they display "ok" and if not, "nok: info" or info is an error message - see below:
add node nid add a nid identifier to the blockchain node.
rm node nid... remove nodes from the blockchain with a nid identifier. If nid is '*', then all nodes are impacted.
add block bid nid... add a bid identifier block to nodes identified by nid. If nid is '*', then all nodes are impacted.
rm block bid... remove the bid identified blocks from all nodes where these blocks are present.
ls list all nodes by their identifiers. The option -l attaches the blocks bid's associated with each node.
sync synchronize all of the nodes with each other. Upon issuing this command, all of the nodes are composed of the same blocks.
quit save and leave the blockchain.
The blockchain prompt must display:<br>
a [ character
a first letter that indicates the state of synchronization of the chain:
"s" if the blockchain is synchronized
"-" if the blockchain is not synchronized.
n number of nodes in the chain.
the "]> " string (with a space)<br>
remember:
- Node id cannot be zero (0)
- Empty command forbidden

## Installation

make

## Usage
After compile write this command.<br>
![ Handbook ](https://drive.google.com/file/d/1SZvS5qtuqq8UcuTfkSgaCuxstq48jF1w/view?usp=sharing)
```
./my_blockchain
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar Silicon Valley</a></i></span>
<span><img alt='Qwasar Silicon Valley Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
