#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <thread>
#include <algorithm>
#include <atomic>
#include <mutex>

struct bbuffer {
    static constexpr size_t bcapacity = 128;
    char bbuf_[bcapacity];
    size_t bpos_ = 0;
    size_t blen_ = 0; //#bytes in the buffer
    bool write_closed_ = false;
    std::mutex mutex_;

    // **** Add condition variables here ****

    ssize_t read(char* buf, size_t sz);
    ssize_t write(const char* buf, size_t sz);
    void shutdown_write();
};

ssize_t bbuffer::write(const char* buf, size_t sz) {
    std::unique_lock<std::mutex> guard(this->mutex_);
    assert(!this->write_closed_);
    size_t pos = 0;

    // **** Add code here: ****
    // If the buffer is initially full (this->blen_ == bcapacity), then
    // writer should block until it becomes nonfull.

    // Copy data from buf to bbuf, no need to understand the details
    while (pos < sz && this->blen_ < bcapacity) {
        size_t bindex = (this->bpos_ + this->blen_) % bcapacity;
        this->bbuf_[bindex] = buf[pos];
        ++this->blen_;
        ++pos;
    }

    // **** Add code here: ****
    // Writer should notify readers if necessary.

    if (pos == 0 && sz > 0) {
        return -1;  // try again
    } else {
        return pos;
    }
}

ssize_t bbuffer::read(char* buf, size_t sz) {
    std::unique_lock<std::mutex> guard(this->mutex_);
    size_t pos = 0;

    // **** Add code here: ****
    // If the buffer is initially empty (this->blen_ == 0), then
    // reader should block until it becomes nonempty or closed.

    // Copy data from bbuf to buf, no need to understand the details
    while (pos < sz && this->blen_ > 0) {
        buf[pos] = this->bbuf_[this->bpos_];
        this->bpos_ = (this->bpos_ + 1) % bcapacity;
        --this->blen_;
        ++pos;
    }

    // **** Add code here: ****
    // Reader should notify writers if necessary.

    if (pos == 0 && sz > 0 && !this->write_closed_) {
        return -1;  // try again
    } else {
        return pos;
    }
}

void bbuffer::shutdown_write() {
    std::unique_lock<std::mutex> guard(this->mutex_);
    this->write_closed_ = true;
    // **** Do you need to notify any threads? ****
}


std::atomic<size_t> nwrites;
std::atomic<size_t> nreads;

void writer_threadfunc(bbuffer& bb) {
    // Write `Hello world!\n` to the buffer a million times.
    // Result should have 13000000 characters.
    const char msg[] = "Hello world!\n";
    const size_t msg_len = strlen(msg);
    for (int i = 0; i != 1000000; ++i) {
        size_t pos = 0;
        while (pos < msg_len) {
            ssize_t nw = bb.write(&msg[pos], msg_len - pos);
            ++nwrites;
            if (nw > -1) {
                pos += nw;
            }
        }
    }
    bb.shutdown_write();
}

void reader_threadfunc(bbuffer& bb) {
    // Read from the buffer until closed and write to stdout.
    char buf[BUFSIZ];
    ssize_t nr;
    while ((nr = bb.read(buf, sizeof(buf))) != 0) {
        ++nreads;
        if (nr > -1) {
            fwrite(buf, 1, nr, stdout);
        }
    }
}


int main() {
    bbuffer bb;
    std::thread reader(reader_threadfunc, std::ref(bb));
    std::thread writer(writer_threadfunc, std::ref(bb));
    reader.join();
    writer.join();
    fprintf(stderr, "%zu reads, %zu writes\n", nreads.load(), nwrites.load());
}
