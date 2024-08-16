class Yolov8Detection:
    def __init__(self, model_path, imgsz, device):
        self.init_model(model_path, imgsz, device)
    
    def init_model(self, model_path, imgsz, device):
        self.model_path = model_path
        self.imgsz = imgsz
        self.device = device

    def detect(self, img):
        # TODO: implement yolov8 detection
        print('model_path:', self.model_path, flush=True)
        print('imgsz:', self.imgsz, flush=True)
        print('device:', self.device, flush=True)
        print("yolov8 detection not implemented yet", flush=True)
        pass