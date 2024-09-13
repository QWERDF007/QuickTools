import torch
from ultralytics.models.yolo.detect import DetectionPredictor

class Yolov8DetectionPredictor:
    def __init__(self, model_path, imgsz, device):
        self.init_model(model_path, imgsz, device)
    
    def init_model(self, model_path, imgsz, device):
        print(f"Loading YOLOv8 model from {model_path} on device {device}...", flush=True)
        self.model_path = model_path
        self.imgsz = imgsz
        self.device = device
        cfg = {'batch': 1, 'model': self.model_path, 'conf': 0.25, 'iou': 0.7, 'imgsz': self.imgsz, 
                'device': self.device,'save_txt': False, 'visualize': False, 'save': False, 'verbose': False}
        self.model = DetectionPredictor(overrides=cfg)

    @torch.no_grad()
    def predict(self, img, conf, iou):
        self.model.args.conf = conf
        self.model.args.iou = iou
        result = self.model(img)
        boxes = result[0].boxes
        ret = {
            'status': 0,
            'msg': 'ok',
            'boxes': boxes.xyxy.tolist(),
            'cls': boxes.cls.int().tolist(),
            'conf': boxes.conf.tolist(),
            'format': 'xyxy',
        }
        print(ret, flush=True)
        return ret
        