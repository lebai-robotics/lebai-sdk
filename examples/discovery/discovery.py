from pylebai import zeroconf


def main():
    discovery = zeroconf.Discovery()
    controllers = discovery.resolve()
    print(f"Found {len(controllers)} controller(s)")
    for controller in controllers:
        print(controller.ip_address, controller.model, controller.hostname)


if __name__ == "__main__":
    main()
