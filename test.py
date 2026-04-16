import typed_bindings


def int_metric(a, b):
    print(f"[Int metric] first type: {type(a)}, second type: {type(b)}")
    return abs(a**2 - b**2)


def str_metric(a, b):
    print(f"[Str metric] first type: {type(a)}, second type: {type(b)}")
    return (ord(a[0]) + ord(b[0])) / (len(a) + len(b))


algo = typed_bindings.Algorithm(typed_bindings.Metric(int_metric), 0)
algo.execute()

algo = typed_bindings.Algorithm(typed_bindings.Metric(str_metric), 3)
algo.execute()
