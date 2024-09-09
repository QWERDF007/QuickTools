from ultralytics import YOLO
from ultralytics.models.yolo.detect import DetectionPredictor

class Yolov8DetectionPredictor:
    def __init__(self, model_path, imgsz, device):
        self.init_model(model_path, imgsz, device)
    
    def init_model(self, model_path, imgsz, device):
        self.model_path = model_path
        self.imgsz = imgsz
        self.device = device
        self.model = YOLO(self.model_path)
        

    def predict(self, img, conf, iou):
        print(img.shape, flush=True)
        result = self.model(img, conf=conf, iou=iou, device=self.device)
        boxes = result[0].boxes
        ret = {
            'status': 0,
            'msg': 'ok',
            'boxes': boxes.xyxy.tolist(),
            'cls': boxes.cls.tolist(),
            'conf': boxes.conf.tolist()
        }
        print(ret, flush=True)
        return ret
        