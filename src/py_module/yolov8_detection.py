class Yolov8Detection:
    def __init__(self, model_path, imgsz, device):
        self.init_model(model_path, imgsz, device)
    
    def init_model(self, model_path, imgsz, device):
        self.model_path = model_path
        self.imgsz = imgsz
        self.device = device