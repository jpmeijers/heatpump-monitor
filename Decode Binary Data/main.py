import binascii


def main():
    with open("24h hp cutecom.log", mode='rb') as file:  # b is important -> binary
        data = file.read()

        # start = 0
        # for i in range(0, len(data)):
        #     if data[i] == 0xAA and data[i+2] == 0xFF:
        #         start = i
        #         break

        i = 0
        while i < len(data) - 65:  # requires a full Q and R left in buffer
            # First frame must be the Q, so AAxxFF... followed by the R
            if data[i] != 0xAA or data[i + 2] != 0xFF:
                i += 1
                continue
            query = data[i:i + 32]
            response = data[i + 32:i + 64]
            # print("Q", binascii.hexlify(query))
            # print("R", binascii.hexlify(response))

            line = []
            line.append("Q")

            for j in range(0, 32):
                # line.append(format(query[j], "02X"))
                line.append(format(query[j], "d"))
            line.append("R")

            for j in range(0, 32):
                # line.append(format(response[j], "02X"))
                line.append(format(response[j], "d"))
            print(",".join(line))
            # print(binascii.hexlify(data[i:i + 64]))

            # if i>5000:
            #     break

            i = i + 64


if __name__ == '__main__':
    main()
