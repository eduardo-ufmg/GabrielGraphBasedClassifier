import argparse
import pathlib
import subprocess
import plot
import metrics
from classifier_pb2 import TrainingDataset, VerticesToLabel, LabeledVertices

def main():
    parser = argparse.ArgumentParser("Train and evaluate multiple classifiers")
    parser.add_argument("--dataset", required=True, help="Path to the dataset")
    parser.add_argument("--tolerance", default=1.0, help="Filter tolerance")
    args = parser.parse_args()
    
    classifiers_dir = pathlib.Path("../bin")
    dataset_name = args.dataset
    dataset_path = pathlib.Path("../data") / dataset_name / dataset_name
    tolerance = str(args.tolerance)
    test_path = pathlib.Path("../data") / dataset_name / "test"
    
    # Load dataset
    pb_dataset = TrainingDataset()
    pb_dataset.ParseFromString(open(dataset_path, "rb").read())

    # Load to-label vertices
    pb_test = VerticesToLabel()
    pb_test.ParseFromString(open(test_path, "rb").read())
    
    # Define classifiers
    classifiers = {
        "chip": {"trainer": "./chip-train", "labeler": "./chip-label"},
        "rchip": {"trainer": "./rchip-train", "labeler": "./rchip-label"},
        "nn": {"trainer": "./nn-train", "labeler": "./nn-label"}
    }
    
    # Storage for labeled results and correctness
    labeled_results = {}
    
    for clf_name, paths in classifiers.items():
        trainer = paths["trainer"]
        labeler = paths["labeler"]
        
        # Train classifier
        train_time = metrics.run_and_measure_time([trainer, str(dataset_path), tolerance], classifiers_dir)

        # Determine file paths for trained model
        trained_model_path = classifiers_dir / "train" / f"{clf_name}-{dataset_name}"

        # Model size
        model_size = trained_model_path.stat().st_size
        
        # Label dataset
        label_time = metrics.run_and_measure_time([labeler, str(test_path), str(trained_model_path)], cwd=classifiers_dir)
        
        # Load labeled results
        labeled_path = classifiers_dir / "label" / f"{clf_name}-{dataset_name}"
        pb_labeled = LabeledVertices()
        pb_labeled.ParseFromString(open(labeled_path, "rb").read())
        
        run_metrics = {
            "train_time": train_time,
            "model_size": model_size,
            "label_time": label_time,
        }
        
        labeled_results[clf_name] = (pb_labeled, run_metrics)

    # Plot results
    plot.plot_classification_results(
        pb_dataset, 
        labeled_results["chip"], 
        labeled_results["rchip"], 
        labeled_results["nn"], 
        dataset_name,
    )

if __name__ == "__main__":
    main()
    