#ifndef _NODE_ESP_H
#define _NODE_ESP_H

class NodeEsp {
public:
	char nombre[30];
	char desc[100];
	char usuario[15];

	bool operator >(const NodeEsp& esp);

	bool operator >=(const NodeEsp& esp);

	bool operator <(const NodeEsp& esp);

	bool operator <=(const NodeEsp& esp);
};

#endif // !_NODE_ESP_H

