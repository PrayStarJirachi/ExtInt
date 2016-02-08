#ifdef EXTINT_H
#ifndef EXTINT_HPP
#define EXTINT_HPP

ExtInt::DListNode & ExtInt::DListNode::remove() {
	DListNode *tmp = this -> forward;
	tmp -> forward -> next = this;
	this -> forward = tmp -> forward;
	delete tmp;
	return *this;
}
ExtInt::DListNode & ExtInt::DListNode::append(DListNode *data) {
	data -> next = this -> next;
	data -> forward = this;
	if (this -> next != NULL) {
		this -> next -> forward = data;
	}
	this -> next = data;
	return *this;
}
ExtInt::DListNode & ExtInt::DListNode::append(const int &data) {
	return append(new DListNode(data));
}
ExtInt::DListNode & ExtInt::DListNode::insert(DListNode *data) {
	data -> next = this;
	data -> forward = this -> forward;
	if (this -> forward != NULL) {
		this -> forward -> next = data;
	}
	this -> forward = data;
	return *this;
}
ExtInt::DListNode & ExtInt::DListNode::insert(const int &data) {
	return insert(new DListNode(data));
}

ExtInt::ExtInt(int data) {
	low = new DListNode;
	high = new DListNode;
	low -> append(high);
	length = 0;
	isNegative = false;
	if (data < 0) {
		isNegative = true;
		data = -data;
	}
	while (data >= ExtInt::LIMIT) {
		high -> insert(data % ExtInt::LIMIT);
		data /= ExtInt::LIMIT;
		length++;
	}
	if (length == 0 || data > 0) {
		high -> insert(data);
		length++;
	}
}
ExtInt::ExtInt(const ExtInt &rhs) {
	low = new DListNode;
	high = new DListNode;
	low -> append(high);
	length = 0;
	isNegative = rhs.isNegative;
	for (DListNode *it = rhs.low -> next; it != rhs.high; it = it -> next) {
		high -> insert(it -> data);
		length++;
	}
}
ExtInt::ExtInt(const std::string &rhs) {
	low = new DListNode;
	high = new DListNode;
	low -> append(high);
	length = 0;
	isNegative = false;
	if (rhs[0] == '-') {
		isNegative = true;
	}
	for (int i = rhs.length() - 1; i >= 0; i -= WIDTH) {
		int value = 0;
		for (int j = std::max(0, i - WIDTH + 1); j <= i; j++) {
			if (j == 0 && isNegative) continue;
			assert(isdigit(rhs[j]));
			value = value * 10 + rhs[j] - '0';
		}
		high -> insert(value);
		length++;
	}
	while (length > 1 && high -> forward -> data == 0) {
		high -> remove();
		length--;
	}
}
ExtInt & ExtInt::operator =(const ExtInt &rhs) {
	if (this == &rhs) return *this;
	if (low != NULL || high != NULL) {
		this -> ~ExtInt();
	}
	low = new DListNode;
	high = new DListNode;
	low -> append(high);
	length = 0;
	isNegative = rhs.isNegative;
	for (DListNode *it = rhs.low -> next; it != rhs.high; it = it -> next) {
		high -> insert(it -> data);
		length++;
	}
	return *this;
}
ExtInt::~ExtInt() {
	for (DListNode *it = low; it != NULL;) {
		DListNode *tmp = it -> next;
		delete it;
		it = tmp;
	}
}

std::istream & operator >>(std::istream &inputStream, ExtInt &data) {
	std::string tmp;
	inputStream >> tmp;
	data = ExtInt(tmp);
	return inputStream;
}
std::ostream & operator <<(std::ostream &outputStream, const ExtInt &data) {
	if (data.isNegative) {
		outputStream << "-";
	}
	ExtInt::DListNode *it = data.high -> forward;
	outputStream << it -> data;
	for (it = it -> forward; it != data.low; it = it -> forward) {
		outputStream << std::setfill('0') << std::setw(ExtInt::WIDTH) << it -> data;
	}
	return outputStream;
}

bool operator <(const ExtInt &a, const ExtInt &b) {
	if (a.isNegative && !b.isNegative) return true;
	if (!a.isNegative && b.isNegative) return false;
	bool flag = false;
	if (a.isNegative && b.isNegative) flag = true;
	if (a.length < b.length) return flag ^ true;
	if (a.length > b.length) return flag ^ false;
	ExtInt::DListNode *itA = a.high, *itB = b.high;
	for (; itA != a.low && itB != b.low; itA = itA -> forward, itB = itB -> forward) {
		if (itA -> data < itB -> data) return flag ^ true;
		if (itA -> data > itB -> data) return flag ^ false;
	}
	return false;
}
bool operator >(const ExtInt &a, const ExtInt &b) {
	if (a.isNegative && !b.isNegative) return false;
	if (!a.isNegative && b.isNegative) return true;
	bool flag = false;
	if (a.isNegative && b.isNegative) flag = true;
	if (a.length < b.length) return flag ^ false;
	if (a.length > b.length) return flag ^ true;
	ExtInt::DListNode *itA = a.high, *itB = b.high;
	for (; itA != a.low && itB != b.low; itA = itA -> forward, itB = itB -> forward) {
		if (itA -> data < itB -> data) return flag ^ false;
		if (itA -> data > itB -> data) return flag ^ true;
	}
	return false;
}
bool operator >=(const ExtInt &a, const ExtInt &b) {
	return !(a < b);
}
bool operator <=(const ExtInt &a, const ExtInt &b) {
	return !(a > b);
}
bool operator ==(const ExtInt &a, const ExtInt &b) {
	if (a.isNegative != b.isNegative) return false;
	if (a.length != b.length) return false;
	ExtInt::DListNode *itA = a.low, *itB = b.low;
	for (; itA != a.high && itB != b.high; itA = itA -> next, itB = itB -> next) {
		if (itA -> data != itB -> data) return false;
	}
	return true;
}
bool operator !=(const ExtInt &a, const ExtInt &b) {
	return !(a == b);
}

ExtInt operator +(const ExtInt &a, const ExtInt &b) {
	if (b.isNegative) {
		ExtInt tmp = b;
		tmp.isNegative = false;
		return a - tmp;
	}
	if (a.isNegative) {
		ExtInt tmp = a;
		tmp.isNegative = false;
		return b - tmp;
	}
	ExtInt ret = a;
	ExtInt::DListNode *itA = ret.low -> next, *itB = b.low -> next;
	for (; itB != b.high; itA = itA -> next, itB = itB -> next) {
		if (itA == ret.high) {
			itA -> insert(0);
			ret.length++;
			itA = itA -> forward;
		}
		itA -> data += itB -> data;
		if (itA -> data >= ExtInt::LIMIT) {
			if (itA -> next == ret.high) {
				itA -> append(0);
				ret.length++;
			}
			itA -> next -> data += itA -> data / ExtInt::LIMIT;
			itA -> data %= ExtInt::LIMIT;
		}
	}
	return ret;
}
ExtInt operator -(const ExtInt &a, const ExtInt &b) {
	if (b.isNegative) {
		ExtInt tmp = b;
		tmp.isNegative = false;
		return a + tmp;
	}
	if (a.isNegative) {
		ExtInt tmp = a;
		tmp.isNegative = false;
		tmp = tmp + b;
		tmp.isNegative = true;
		return tmp;
	}
	if (a < b) {
		ExtInt tmp = b - a;
		tmp.isNegative = true;
		return tmp;
	}
	ExtInt ret = a;
	ExtInt::DListNode *itA = ret.low -> next, *itB = b.low -> next;
	for (; itA != ret.high && itB != b.high; itA = itA -> next, itB = itB -> next) {
		itA -> data -= itB -> data;
		if (itA -> data < 0) {
			itA -> data += ExtInt::LIMIT;
			itA -> next -> data--;
		}
	}
	for (; itA != ret.high; itA = itA -> next) {
		if (itA -> data < 0) {
			itA -> data += ExtInt::LIMIT;
			itA -> next -> data--;
		}
	}
	while (ret.length > 1 && ret.high -> forward -> data == 0) {
		ret.high -> remove();
		ret.length--;
	}
	return ret;
}
ExtInt operator *(const ExtInt &a, const ExtInt &b) {
	if (a == ExtInt(0) || b == ExtInt(0)) return ExtInt(0);
	ExtInt ret, tmp;
	ExtInt::DListNode *itB = b.low -> next;
	for (int value = 0; itB != b.high; itB = itB -> next, value++) {
		if (itB -> data == 0) {
			continue;
		}
		tmp = a;
		for (ExtInt::DListNode *itA = tmp.low -> next; itA != tmp.high; itA = itA -> next) {
			itA -> data *= itB -> data;
		}
		for (ExtInt::DListNode *itA = tmp.low -> next; itA != tmp.high; itA = itA -> next) {
			if (itA -> data >= ExtInt::LIMIT) {
				if (itA -> next == tmp.high) {
					itA -> append(0);
					tmp.length++;
				}
				itA -> next -> data += itA -> data / ExtInt::LIMIT;
				itA -> data %= ExtInt::LIMIT;
			}
		}
		for (int i = 1; i <= value; i++) {
			tmp.low -> append(0);
			tmp.length++;
		}
		//std::cerr << ret << std::endl;
		//std::cerr << tmp << std::endl;
		//std::cerr << tmp.length << std::endl;
		ret = ret + tmp;
	}
	ret.isNegative = a.isNegative ^ b.isNegative;
	return ret;
}
ExtInt operator /(const ExtInt &a, const ExtInt &b) {
	assert(b != ExtInt(0));
	if (a == ExtInt(0)) return ExtInt(0);
	ExtInt ret, tmp, div = b;
	ret.high -> remove();
	ret.length = 0;
	tmp.high -> remove();
	tmp.length = 0;
	div.isNegative = false;
	ExtInt::DListNode *itA = a.high -> forward;
	for (; itA != a.low; itA = itA -> forward) {
		tmp.low -> append(itA -> data);
		tmp.length++;
		if (tmp >= div) {
			int left = 0, right = ExtInt::LIMIT - 1;
			while (left < right) {
				int middle = (left + right >> 1) + 1;
				if (tmp >= div * middle) {
					left = middle;
				} else {
					right = middle - 1;
				}
			}
			//std::cerr << tmp << " " << div * left << std::endl;
			ret.low -> append(left);
			ret.length++;
			tmp = tmp - div * left;
			if (tmp == ExtInt(0)) {
				tmp.high -> remove();
				tmp.length = 0;
			}
		} else {
			ret.low -> append(0);
			ret.length++;
		}
	}
	while (ret.length > 1 && ret.high -> forward -> data == 0) {
		ret.high -> remove();
		ret.length--;
	}
	ret.isNegative = a.isNegative ^ b.isNegative;
	if (ret.isNegative && tmp.low -> next != tmp.high) {
		ret = ret - 1;
	}
	return ret;
}
ExtInt operator %(const ExtInt &a, const ExtInt &b) {
	return a - a / b * b;
}

ExtInt & ExtInt::operator +=(const ExtInt &rhs) {
	*this = *this + rhs;
	return *this;
}
ExtInt & ExtInt::operator -=(const ExtInt &rhs) {
	*this = *this + rhs;
	return *this;
}
ExtInt & ExtInt::operator *=(const ExtInt &rhs) {
	*this = *this + rhs;
	return *this;
}
ExtInt & ExtInt::operator /=(const ExtInt &rhs) {
	*this = *this + rhs;
	return *this;
}
ExtInt & ExtInt::operator %=(const ExtInt &rhs) {
	*this = *this + rhs;
	return *this;
}

#endif
#endif
