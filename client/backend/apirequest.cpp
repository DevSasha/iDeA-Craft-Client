#include "apirequest.h"

APIRequest::APIRequest(QString method) {
	if (this->mng == nullptr) {
		this->mng = new QNetworkAccessManager;
	}
}
