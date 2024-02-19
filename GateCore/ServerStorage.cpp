#include "ServerStorage.h"

ServerStorage::ServerStorage() {
    this->onGetResponse = [](String data) {};
}

String ServerStorage::oneParam(String p1) {
    return String(p1.length()) + "|" + p1;
}

String ServerStorage::twoParams(String p1, String p2) {
    return String(p1.length()) + "," + String(p2.length()) + "|" + p1 + p2;
}

String ServerStorage::threeParams(String p1, String p2, String p3) {
    return String(p1.length()) + "," + String(p2.length()) + "," + String(p3.length()) + "|" + p1 + p2 + p3;
}

void ServerStorage::get(String key, StorageGetCallback onResponse) {
    if (key.length() > 0) {
        this->onGetResponse = onResponse;
        String message = "*?storageGet|" + this->oneParam(key);
        this->device->send(message);
    }
}

void ServerStorage::get(String key, String directory, StorageGetCallback onResponse) {
    if (key.length() > 0) {
        if (directory.length() > 0) {
            this->onGetResponse = onResponse;
            String message = "*?storageGet|" + this->twoParams(key, directory);
            this->device->send(message);
        } else {
            this->get(key, onResponse);
        }
    }
}

void ServerStorage::set(String key, String value) {
    if (key.length() > 0 && value.length() > 0) {
        String message = "*!storageSet|" + this->twoParams(key, value);
        this->device->send(message);
    }
}

void ServerStorage::set(String key, String value, String directory) {
    if (key.length() > 0 && value.length() > 0) {
        if (directory.length() > 0) {
            String message = "*!storageSet|" + this->threeParams(key, value, directory);
            this->device->send(message);
        } else {
            this->set(key, value);
        }
    }
}

void ServerStorage::append(String key, String value) {
    if (key.length() > 0 && value.length() > 0) {
        String message = "*!storageAppend|" + this->twoParams(key, value);
        this->device->send(message);
    }
}

void ServerStorage::append(String key, String value, String directory) {
    if (key.length() > 0 && value.length() > 0) {
        if (directory.length() > 0) {
            String message = "*!storageAppend|" + this->threeParams(key, value, directory);
            this->device->send(message);
        } else {
            this->set(key, value);
        }
    }
}

void ServerStorage::remove() {
    this->remove("");
}

void ServerStorage::remove(String key) {
    String message = "*!storageRemove|" + this->oneParam(key);
    this->device->send(message);
}

void ServerStorage::remove(String key, String directory) {
    if (directory.length() == 0) {
        this->remove(key);
    } else {
        String message = "*!storageRemove|" + this->twoParams(key, directory);
        this->device->send(message);
    }
}

void ServerStorage::handleGetResponse(String message) {
    String value = message.substring(13);
    this->onGetResponse(value);
}